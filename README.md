# 🌐 Despliegue de una red de distribución IPTV Multicast

### Planificación y Simulación de Redes (PSR)

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![ns-3](https://img.shields.io/badge/ns--3-3.48-blue?style=flat)
![MPEG-TS](https://img.shields.io/badge/Format-MPEG--TS-blue?style=flat)
![UDP](https://img.shields.io/badge/Protocol-UDP-orange?style=flat)
![IPv4](https://img.shields.io/badge/Network-IPv4-lightgrey?style=flat)
![Multicast](https://img.shields.io/badge/Network-Multicast-purple?style=flat)

## 📌 Descripción

Este proyecto aborda el diseño y la evaluación de una infraestructura de red para la distribución de televisión en directo mediante **IPTV multicast** en un distrito de nueva construcción.

La arquitectura propuesta sigue una estructura jerárquica de operador organizada en tres niveles: núcleo, agregación y acceso. Mediante el simulador **ns-3**, se modela la topología de red y el comportamiento del tráfico multicast generado por los servidores de distribución.

El objetivo principal es determinar la capacidad mínima necesaria en los enlaces del núcleo para satisfacer los requisitos de calidad de servicio (QoS), minimizando los costes de despliegue y manteniendo la posibilidad de adaptar la infraestructura a otros escenarios.

## 🎯 Objetivos

- Diseñar una arquitectura de distribución IPTV multicast.
- Modelar una infraestructura de red organizada en núcleo, agregación y acceso.
- Implementar la generación y distribución de flujos multicast sobre UDP/IPv4.
- Configurar las características de los enlaces y los parámetros de transmisión.
- Estudiar el efecto de la demanda de canales sobre la carga de la red.
- Evaluar las prestaciones de la infraestructura mediante ns-3.
- Analizar las pérdidas de paquetes, el retardo y el jitter.
- Determinar el ancho de banda mínimo de los enlaces del núcleo que permita cumplir los requisitos de QoS.

## 🏗️ Arquitectura de red

La infraestructura se divide en tres niveles:

| Nivel | Elementos | Función |
|---|---|---|
| Núcleo | Routers N | Interconexión de los servidores de distribución y transporte del tráfico. |
| Agregación | Routers A | Concentración y distribución de los flujos hacia las distintas urbanizaciones. |
| Acceso | Routers AC | Distribución de los canales multicast hacia los edificios. |

Los servidores de distribución generan los flujos correspondientes a los distintos canales de televisión. La tecnología multicast permite compartir un mismo flujo entre múltiples receptores, evitando transmisiones independientes para cada usuario.

## 🛠️ Tecnologías y protocolos

| Tecnología | Aplicación |
|---|---|
| **C++** | Desarrollo del escenario de red. |
| **ns-3.48** | Modelado y evaluación de la infraestructura. |
| **IPv4** | Direccionamiento y encaminamiento IP. |
| **UDP** | Transporte de los flujos de vídeo. |
| **IP Multicast** | Distribución de tráfico a múltiples receptores. |
| **IEEE 802.3** | Tecnología de enlace Ethernet. |
| **CSMA/CD** | Modelado de los enlaces compartidos de acceso. |
| **IGMP** | Gestión de pertenencia a grupos multicast en el sistema de referencia. |
| **gnuplot** | Representación gráfica de los resultados. |

## 📊 Evaluación de QoS

La evaluación se basa en los objetivos de rendimiento de la **Provisional QoS Class 7** de la recomendación ITU-T Y.1541.

Se analizan tres métricas principales:

- **Pérdida de paquetes:** porcentaje de paquetes que no llegan a su destino.
- **Retardo:** tiempo de tránsito de los paquetes a través de la red.
- **Jitter:** variación del retardo entre paquetes.

El dimensionamiento de los enlaces se realiza buscando configuraciones que permitan satisfacer simultáneamente los requisitos establecidos para las tres métricas.

## 🧪 Diseño experimental

Se estudia el comportamiento de la red modificando diferentes parámetros de configuración, entre ellos:

- Capacidad de transmisión de los enlaces.
- Calidad de los canales de vídeo.
- Demanda de canales por parte de los routers de acceso.
- Distribución de los usuarios entre los edificios.
- Retardos de propagación.

Los experimentos permiten comparar distintas configuraciones y analizar su impacto sobre las prestaciones de la infraestructura.

## 💻 Entorno de desarrollo

| Herramienta | Versión / Entorno |
|---|---|
| ns-3 | 3.48 |
| C++ | Lenguaje de implementación |
| WSL | Entorno Linux sobre Windows |
| gnuplot | Visualización de resultados |

## 📁 Estructura del repositorio

La organización de los directorios y ficheros se irá completando conforme se incorporen los distintos componentes del proyecto.

## 📚 Referencias


[1] Norma ITU-T Y.1541 - Network performance objectives for IP-based services: https://www.itu.int/rec/T-REC-Y.1541-201112-I/es

[2] RFC 1112 - Host extensions for IP multicasting: https://www.rfc-editor.org/info/rfc1112/

[3] RFC 2236 - Internet Group Management Protocol, Version 2: https://www.rfc-editor.org/rfc/rfc2236.txt

[4] RFC 4601 - Protocol Independent Multicast - Sparse Mode (PIM-SM): Protocol Specification (Revised): https://www.rfc-editor.org/rfc/rfc4601.txt

[5] Digital Video Broadcasting (DVB); Transport of MPEG-2 TS Based DVB Services over IP Based Networks: https://www.etsi.org/deliver/etsi_ts/102000_102099/102034/01.04.01_60/ts_102034v010401p.pdf

[6] Digital Video Broadcasting (DVB) - Specification for the use of Video and Audio Coding in Broadcasting Applications based on the MPEG-2 Transport Stream: https://www.etsi.org/deliver/etsi_ts/101100_101199/101154/01.08.01_60/ts_101154v010801p.pdf

[7] Norma ITU-T H.610 - VSDL de servicio completo – Arquitectura de sistema y del equipo en las instalaciones del cliente: https://www.itu.int/rec/T-REC-H.610-200307-I/es

[8] IEEE Standards Association. IEEE Standard for Ethernet: https://standards.ieee.org/ieee/7003/10422/

[9] Norma ITU-T H.264 - Advanced video coding for generic audiovisual services: https://www.itu.int/rec/T-REC-H.264-202606-I/es

[10] Norma ITU-T H.265 - High efficiency video coding: https://www.itu.int/rec/T-REC-H.265-202601-I/es

