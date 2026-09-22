# SapFlow Logger

Open-source **field logger for Granier-style thermal dissipation sap-flow measurements**, with onboard microSD storage and **Bluetooth Low Energy** download to a phone.

Built by [Rennan Andres Paloschi](https://github.com/Paloschi) for ecohydrology / plant water-use campaigns (Caatinga and related dry forests).

| | |
| --- | --- |
| **Platform** | ESP32 (Arduino / `.ino` firmware) |
| **Sensing** | Thermocouple front-end (`Nanoshield_Termopar`) for probe ΔT |
| **Power monitor** | INA219 |
| **Clock** | DS3231 RTC (`RTClib`) |
| **Storage** | microSD (SPI) — CSV-style append logging |
| **Download** | BLE GATT service — list files / send file / sync clock from phone |
| **License** | MIT |

> Firmware revision packaged here: **Hardware 6.0** (`firmware/`).

---

## Why this exists

Station loggers are great for eddy-covariance towers, but sap-flow plots often need **many cheap autonomous nodes**. This logger:

1. Records probe / power / clock data to **microSD** in the field  
2. Lets you walk up with a phone, connect over **BLE**, list files, and pull data without opening the box  
3. Supports clock sync from the companion app over BLE  

A production batch of **10 units** was built and delivered for Caatinga tree water-use fieldwork (UFRPE collaborators).

---

## Demo video

Public walkthrough of how to use the logger in the field (BLE download / phone workflow):

**https://www.youtube.com/watch?v=al5Gi5oz8YY&t=558s**

## Firmware layout

```
firmware/
├── firmware.ino      # setup/loop, module glue ("LOGGER DE FLUXO DE SEIVA")
├── bluetooth.ino     # BLE server, file list/send, clock update characteristics
├── microSD.ino       # SD mount + append logging
├── termopar.ino      # thermocouple read path
├── ina219.ino        # power monitoring
├── relogio.ino       # RTC helpers
├── RTClib.cpp/.h     # DS3231 RTC library (bundled)
```

Build with the Arduino IDE / ESP32 core. Install library dependencies used by the sketch (`FS`, `SD`, `SPI`, `Adafruit_INA219`, `Nanoshield_Termopar`, BLE stack from ESP32 core).

---

## Field use & related publications

These loggers were designed, built, and supplied (10 units: **SD + Bluetooth + mobile app**) for sap-flow / tree water-use campaigns associated with the group of **Prof. Eduardo Soares de Souza** (UFRPE) and collaborators, including fieldwork involving **Angela Lucena Nascimento de Jesus**.

Papers from those campaigns (and the same group’s sap-velocity / canopy water-use line of work):

1. **Medeiros, M.**; Lima, A. L. A.; Silva, J. R. I.; **Jesus, A. L. N.**; Wright, C. L.; **Souza, E. S.**; Santos, M. G. (2025). Seasonal shifts in tree water use and non-structural carbohydrate storage in a tropical dry forest. *Plant, Cell & Environment*, 48(6), 4518–4532. https://doi.org/10.1111/pce.15449  

2. **Jesus, A. L. N.**; Brito, N. D. S.; Medeiros, M.; **Souza, E. S.**; Lima, A. L. A. (2025). Semiarid woody plant functional groups contribute differently to canopy conductance. *Journal of Arid Environments*. https://doi.org/10.1016/j.jaridenv.2025.105318  

**Citation honesty:** those articles describe **Granier-style thermal dissipation sensors** (and site meteorology on Campbell equipment where applicable). They do **not** currently name this open-source logger in Methods or Acknowledgements. This repository is the public record of the **field electronics that were built and delivered** for that style of campaign. If you use this hardware or firmware, please cite **this repository** (and optionally the papers above for scientific context).

---

## Citing this project

```bibtex
@software{paloschi_sapflow_logger,
  author  = {Paloschi, Rennan Andres},
  title   = {SapFlow Logger: ESP32 Granier-style field logger with SD and Bluetooth},
  year    = {2026},
  url     = {https://github.com/Paloschi/sapflow-logger},
  version = {6.0}
}
```

---

## Related open work

- [AquaCrop-Grid](https://github.com/Paloschi/aquacrop-grid) — raster AquaCrop / crop water balance  
- [CyMP](https://github.com/Paloschi/CyMP) — crop-yield modeling platform  

---

## Disclaimer

Research instrumentation — validate probes and calibration for your species and climate before publishing fluxes. Do not commit secrets, production keys, or third-party field datasets without permission.
