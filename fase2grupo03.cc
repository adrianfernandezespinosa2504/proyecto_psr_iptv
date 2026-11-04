//
// Red de distribución IPTV multicast de un distrito de nueva construcción
// Topología: nodos, pila de protocolos, enlaces y direcciones IP
//
// Planificación y simulación de redes
//
//
 
#include <vector>
#include <string>
#include <sstream>
 
#include "ns3/command-line.h"
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/node-container.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/data-rate.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"   // UdpClient, UdpServer
#include "ns3/random-variable-stream.h"
#include "ns3/uinteger.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
 
using namespace ns3;
 
// Ctes del programa
const uint32_t num_servidores       = 4;
const uint32_t num_routerAporN      = 4;
const uint32_t num_routerA          = num_servidores * num_routerAporN;
const uint32_t mapa_routerAC[num_routerA] = {4, 3, 4, 2,
                                             4, 4, 2, 3,
                                             3, 4, 2, 4,
                                             2, 3, 4, 2};
const uint32_t num_canalesPorServidor = 40;
const uint32_t num_canales          = num_servidores * num_canalesPorServidor;
const uint32_t num_viviendasPorAC   = 40;
const uint32_t TAM_DATAGRAMA        = 1316;
const uint16_t PUERTO_BASE          = 5000;
const char * const GRUPO_BASE       = "239.0.0.1";
const Time     DURACION             = Time ("122s");

const DataRate CAP_S_N    = DataRate ("2.5Gbps");
const DataRate CAP_N_A    = DataRate ("5Gbps");
const DataRate CAP_A_AC   = DataRate ("5Gbps");
const Time     RETARDO_S_N  = Time ("0.5us");    // 100 m
const Time     RETARDO_N_N  = Time ("10us");     // 2 km
const Time     RETARDO_N_A  = Time ("5us");      // 1 km
const Time     RETARDO_A_AC = Time ("1us");      // 200 m
const char * const TAM_COLA = "100p";
 
NS_LOG_COMPONENT_DEFINE ("Fase2grupo03");
 
int main (int argc, char *argv[])
{
  // Linea de comandos
  DataRate capacidadNN = DataRate ("100Mbps"); // Empieza con el valor mínimo
  Time intervalo = Time ("5.264ms");      // SD; HD 2.632ms, FHD 1.316ms, UHD 0.351ms

  /* Queda por desarrollar 
  CommandLine cmd;
  cmd.AddValue ("num_servidores", "Escenario: Número de servidores (y de routers N)", num_servidores);
  cmd.AddValue ("num_routerAporN", "Escenario: Número de routers A por cada router N", num_routerAporN);
  cmd.AddValue ("mapa_routerAC", "Escenario: Routers AC de cada router A, separados por comas", mapa_routerAC);
  cmd.AddValue ("capacidadNN", "Escenario: Capacidad de los enlaces N-N", capacidadNN);
  cmd.Parse (argc, argv);
  */

  // Creamos los nodos
  NodeContainer c_servidores (num_servidores);
  NodeContainer c_routersN (num_servidores);
  NodeContainer c_routersA (num_routerA);
  std::vector<NodeContainer> c_routersAC (num_routerA);
  for (uint32_t a = 0; a < num_routerA; a++)
    {
      c_routersAC[a].Create (mapa_routerAC[a]);
      NS_LOG_INFO("Creando urbanización "<< a+1 << " con " << mapa_routerAC[a] << " routers AC" );
    }
 
  // Instalamos la pila de protocolos en todos los nodos
  InternetStackHelper h_pila;
  h_pila.SetIpv6StackInstall (false);
  h_pila.Install (c_servidores);
  h_pila.Install (c_routersN);
  h_pila.Install (c_routersA);
  for (uint32_t a = 0; a < num_routerA; a++)
    {
      h_pila.Install (c_routersAC[a]);
      NS_LOG_INFO("Se ha instalado la torre de protocolos en los " << c_routersAC[a].GetN() << " Routers AC asociados al Router A"<< a+1);
    }
 
  // Un generador de direcciones por tipo de enlace
  Ipv4AddressHelper h_ipSN ("10.1.0.0", "255.255.255.252");
  Ipv4AddressHelper h_ipNN ("10.2.0.0", "255.255.255.252");
  Ipv4AddressHelper h_ipNA ("10.3.0.0", "255.255.255.252");
  Ipv4AddressHelper h_ipAAC ("10.4.0.0", "255.255.255.0");
 
  // Para quitar la cola de tráfico que se instala por defecto al asignar las IP
  // TrafficControlHelper h_tc;
  NetDeviceContainer c_dispos;
 
  // Enlaces servidor - router N
  PointToPointHelper h_punto;
  h_punto.SetQueue ("ns3::DropTailQueue<Packet>", "MaxSize", StringValue (TAM_COLA));
  h_punto.SetDeviceAttribute ("DataRate", DataRateValue (CAP_S_N));
  h_punto.SetChannelAttribute ("Delay", TimeValue (RETARDO_S_N));
  for (uint32_t n = 0; n < num_servidores; n++)
    {
      c_dispos = h_punto.Install (c_servidores.Get (n), c_routersN.Get (n));
      h_ipSN.Assign (c_dispos);
      h_ipSN.NewNetwork (); // Para crear una subred nueva
      //h_tc.Uninstall (c_dispos);
      NS_LOG_INFO("Creando enlace entre Servidor "<< n+1 << " con Router N" <<n+1);
    }
 
  // Enlaces entre los routers N, en línea (objeto de estudio)
  h_punto.SetDeviceAttribute ("DataRate", DataRateValue (capacidadNN));
  h_punto.SetChannelAttribute ("Delay", TimeValue (RETARDO_N_N));
  for (uint32_t n = 0; n + 1 < num_servidores; n++)
    {
      c_dispos = h_punto.Install (c_routersN.Get (n), c_routersN.Get (n + 1));
      h_ipNN.Assign (c_dispos);
      h_ipNN.NewNetwork ();
      NS_LOG_INFO("Creando enlace PPP entre Router N"<< n+1 << " con Router N" <<n+2);
      //h_tc.Uninstall (c_dispos);
    }
 
  // Enlaces router N - router A
  h_punto.SetDeviceAttribute ("DataRate", DataRateValue (CAP_N_A));
  h_punto.SetChannelAttribute ("Delay", TimeValue (RETARDO_N_A));
  for (uint32_t n = 0; n < num_servidores; n++) // Por cada router N
  {
    for (uint32_t k = 0; k < num_routerAporN; k++) // En cada router A unido al router N
      {
        uint32_t a = n * num_routerAporN + k;     // número del router A
        c_dispos = h_punto.Install (c_routersN.Get (n), c_routersA.Get (a));
        h_ipNA.Assign (c_dispos);
        h_ipNA.NewNetwork ();
        NS_LOG_INFO("Creando enlace PPP entre Router N"<< n+1 << " con Router A" <<a+1);
        //h_tc.Uninstall (c_dispos);
      }
  }

  // Un canal CSMA por urbanización, compartido por su router A y sus routers AC
  CsmaHelper h_csma;
  h_csma.SetQueue ("ns3::DropTailQueue<Packet>", "MaxSize", StringValue (TAM_COLA));
  h_csma.SetChannelAttribute ("DataRate", DataRateValue (CAP_A_AC));
  h_csma.SetChannelAttribute ("Delay", TimeValue (RETARDO_A_AC));
  for (uint32_t a = 0; a < num_routerA; a++) // Por cada router A
    {
      NodeContainer c_canal (c_routersA.Get(a)); //Router A en primera posicion
      c_canal.Add (c_routersAC[a]); //Añade en orden los routers AC
      c_dispos = h_csma.Install (c_canal);
      h_ipAAC.Assign (c_dispos);
      h_ipAAC.NewNetwork ();
      NS_LOG_INFO("Creando enlace CSMA entre Router A"<< a+1 << " con " << c_routersAC[a].GetN() << " Routers AC");
      //h_tc.Uninstall (c_dispos);
    }
  
  Ptr<UniformRandomVariable> v_aleatoria = CreateObject<UniformRandomVariable> ();
 
  // Un UdpClient por canal en el servidor que lo ofrece (canal -> grupo y puerto)
  for (uint32_t s = 0; s < num_servidores; s++)
    {
      for (uint32_t c = 1; c <= num_canalesPorServidor; c++)
        {
          uint32_t idCanal = s * num_canalesPorServidor + c;
          Ipv4Address grupo = Ipv4Address (Ipv4Address (GRUPO_BASE).Get () + idCanal);
 
          Ptr<UdpClient> cliente_udp = CreateObject<UdpClient> ();
          cliente_udp->SetRemote (InetSocketAddress (grupo, PUERTO_BASE + idCanal));
          cliente_udp->SetAttribute ("MaxPackets", UintegerValue (0));
          cliente_udp->SetAttribute ("Interval", TimeValue (intervalo));
          cliente_udp->SetAttribute ("PacketSize", UintegerValue (TAM_DATAGRAMA));
          c_servidores.Get(s)->AddApplication (cliente_udp);
          cliente_udp->SetStartTime (Time(0)); //
          cliente_udp->SetStopTime (DURACION);
          NS_LOG_INFO("Creando canal "<<idCanal<< " en el Servidor " << s << " con dirección: " << grupo << " en el puerto: " << PUERTO_BASE + idCanal);
        }
    }
 
  // Un UdpServer por cada canal que pide cada router AC: k canales distintos
  // entre los 160, con k entre 1 y 40
  for (uint32_t a = 0; a < num_routerA; a++) //Por cada router A 
    {
      for (uint32_t j = 0; j < mapa_routerAC[a]; j++) //Recorre los routers AC relacionado a un router A
        {
          uint32_t k = v_aleatoria->GetInteger (1, num_viviendasPorAC); // Se elige cuantos canales solicta ese AC
          std::vector<bool> solicitado (num_canales, false); //Se hace una lista con los 160 canales marcados como false (no elegidos)
          uint32_t instalados = 0;
          while (instalados < k)
            {
              uint32_t idCanal = v_aleatoria->GetInteger (1, num_canales);
              if (!solicitado[idCanal])
                {
                  solicitado[idCanal] = true;
                  instalados++;
 
                  Ptr<UdpServer> servidor_udp = CreateObject<UdpServer> ();
                  servidor_udp->SetAttribute ("Port", UintegerValue (PUERTO_BASE + idCanal));
                  c_routersAC[a].Get (j)->AddApplication (servidor_udp);
                  servidor_udp->SetStartTime (Time (0));
                  servidor_udp->SetStopTime (DURACION);
                  NS_LOG_INFO("Router AC" << a+1 <<"-"<< j+1 << " solicitando canal "<<idCanal);
                }
            }
        }
    }

  
  NS_LOG_INFO ("Nodos creados: " << NodeList::GetNNodes ());
  Simulator::Run ();
  Simulator::Destroy ();
}