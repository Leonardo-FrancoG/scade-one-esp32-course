# SCADE One → ESP32: A Model-Based Embedded Systems Course

A nine-practice laboratory series that takes a student from a first SCADE One
operator to a fail-safe **flight-control demonstrator** running on an ESP32
under FreeRTOS — built entirely on one principle:

> **The model owns the logic. The platform owns the I/O.**

Every practice generates C code from a SCADE One model and wraps it in a thin,
auditable *glue layer* on the ESP32, never editing the generated code. Across
the series the glue grows from three blinking LEDs into a priority-scheduled
RTOS application driving four control surfaces, a landing gear, panel
indicators, telemetry, and an in-model emergency mode.

**Author:** Leonardo Franco García · 
**Institution:** Universidad Aeronáutica de Querétaro (UNAQ) — Embedded Systems Laboratory
**Stack:** SCADE One (Ansys) · ESP-IDF v5.x · FreeRTOS · ESP32

---

## The learning arc

| # | Practice | What you build | Key concept |
|---|----------|----------------|-------------|
| 1 | SCADE One Fundamentals | First operators (counter, saturation) | Synchronous dataflow, `pre`, simulation |
| 2 | ESP32 Environment Setup | Toolchain + first blink | ESP-IDF project anatomy, `idf.py`, `ESP_LOG` |
| 3 | First Bridge: SCADE → ESP32 | Traffic light on 3 LEDs | Code generation, `reset`/`step`, the glue layer |
| 4 | Framed UART Telemetry | Binary telemetry frame + loopback | Framing, checksums, `esp_driver_uart` |
| 5 | The Model as a Task | Model inside a FreeRTOS task | Tasks, queues, mutex, `vTaskDelayUntil` |
| 6 | Flight-Phase State Machine | Ground→Takeoff→Cruise→Landing automaton | SCADE state machines, guarded transitions |
| 7 | Sensors & Actuators | Phase-commanded landing gear | ADC (potentiometer) + PWM servo (LEDC) |
| 8 | Fail Safe | Fault detection + emergency state + watchdog | Functional safety, `esp_task_wdt` |
| 9 | Integration Project | Full aircraft: 4 surfaces + gear + emergency | Multi-task RTOS integration |

The same model is carried forward and extended rather than rebuilt: the traffic
light lives through Practice 5, the flight-phase machine of Practice 6 commands
the gear in Practice 7, and the emergency logic of Practice 8 is part of the
complete model in Practice 9.

---

## The final system (Practice 9)

A benchtop flight-control demonstrator implemented as five cooperating FreeRTOS
tasks around a single generated SCADE model:

- **2 potentiometers** — pilot inputs (`mode` = flight phase, `dir` = direction)
- **5 servos** — left/right ailerons, rudder, elevator, and landing gear
- **3 LEDs** — panel status (green / white / red-alarm)
- **In-model emergency** — detects a disconnected or jumping input, forces the
  surfaces to neutral and flashes red
- **Task watchdog** — resets a hung controller as a last resort

| Signal | Role | Pin |
|--------|------|-----|
| `mode` | flight-phase pot (ADC1) | GPIO34 |
| `dir` | direction pot (ADC1) | GPIO35 |
| `aleizq` / `aleder` | ailerons | GPIO13 / GPIO12 |
| `tdir` / `elev` | rudder / elevator | GPIO14 / GPIO27 |
| `lgr` | landing gear | GPIO4 |
| red / green / white | panel LEDs | GPIO25 / 26 / 33 |

> **Power note:** five SG90 servos need an external 5 V supply (≥ 2 A) with its
> ground tied to the ESP32 ground. Do not power servos from the 3V3 pin.

---

## Repository structure

```
.
├── README.md
├── LICENSE
├── practices/            # the 9 lab manuals
│   ├── 01-fundamentals/          (PDF + LaTeX source + images)
│   ├── 02-esp32-setup/
│   ├── ...
│   └── 09-integration/
├── code/                 # ESP-IDF projects (glue layer)
│   ├── p3-traffic-light/
│   ├── p4-telemetry/
│   ├── p5-rtos/
│   ├── p6-flight-phase/
│   ├── p7-landing-gear/
│   ├── p8-fail-safe/
│   └── p9-aircraft/              (main.c, CMakeLists, components/scade_gen)
```

---

## How to use

**Read the manuals** (`practices/`) in order — each PDF is a self-contained
laboratory session with theory, a worked example, a main activity, an
assessment questionnaire, and a troubleshooting section drawn from real
development.

**Build a practice's firmware:**

```bash
# with ESP-IDF v5.x installed and exported
cd code/p9-aircraft
idf.py set-target esp32
idf.py -p COM8 flash monitor      # replace COM8 with your port
```

Each project's `components/scade_gen/` holds the SCADE-generated C plus the
hand-written `swan_config.h` platform-adaptation header. To change behaviour,
edit the SCADE model, regenerate, and copy the new code in — never edit the
generated files by hand.

---

## Requirements

### Software

- **SCADE One** (Ansys) with the Swan code generator — student edition is sufficient
- **ESP-IDF v5.x** (Espressif toolchain: Python, CMake, Ninja, Xtensa GCC)
- A serial terminal (the built-in `idf.py monitor` is enough)

### Hardware (bill of materials)

| # | Item | Qty | Used in | Notes |
|---|------|-----|---------|-------|
| 1 | ESP32 DevKit V1 | 1 | all | USB data cable (not charge-only) |
| 2 | Breadboard | 1–2 | P3+ | half-size is fine; two help for P9 |
| 3 | Jumper wires (M–M, M–F) | ~20 | P3+ | signal, power, and ground |
| 4 | LEDs (red, yellow/white, green) | 3 | P3+ | phase / panel indicators |
| 5 | Resistors 220 Ω | 3 | P3+ | LED current limiting |
| 6 | Jumper (loopback) | 1 | P4, P5 | UART2 loopback GPIO4↔GPIO5 |
| 7 | SG90 micro servo | 1 → 5 | P7 → P9 | 1 in P7, up to 5 in P9 |
| 8 | Potentiometer 10 kΩ | 1 → 2 | P7 → P9 | analog inputs on ADC1 |
| 9 | External 5 V supply (≥ 2 A) | 1 | P7+ | for the servos — **not** the 3V3 pin |

> **Notes on power and pins:** the on-board `BOOT` button (GPIO0) is the input in
> P6–P8, so no button hardware is needed. Servos must be powered from an
> external 5 V rail whose ground is tied to the ESP32 ground; five SG90s moving
> together can draw over 1 A. Potentiometers read on ADC1 pins (GPIO32–39) to
> avoid the Wi-Fi conflict of ADC2.

---

## What you'll learn

By working through the series you get hands-on practice with:

- **Model-based design with SCADE One** — dataflow operators, the `pre`
  memory, state machines with guarded transitions, and code generation.
- **The generator contract** — how a SCADE model compiles to a
  `context` / `reset` / `step` C API, and how to integrate it as an
  ESP-IDF component without editing the generated files.
- **Bare-metal I/O on the ESP32** — GPIO, UART, the ADC one-shot driver, and
  PWM servo control with LEDC.
- **Concurrency with FreeRTOS** — tasks, priorities, queues, mutexes, and
  fixed-period scheduling with `vTaskDelayUntil`.
- **A basic functional-safety mindset** — separating fault *detection* from
  fault *reaction*, a modeled emergency state, and the task watchdog.
- **Debugging real hardware** — brown-outs, wiring faults, and the kind of
  issues documented in each practice's troubleshooting section.

The goal is not a finished product but a clear, worked path from a first
operator to a small concurrent control system, with each design decision
made explicit.

---

## License

See [`LICENSE`](LICENSE). The laboratory manuals (text and figures) are shared
for educational use; the code is provided as-is for learning.

---

*Built as a graduate embedded-systems course at UNAQ — from a first dataflow
operator to a fail-safe flight-control demonstrator, one auditable layer at a
time. An educational prototype that applies model-based, safety-aware design
practices; it is not a certified avionics system.*
