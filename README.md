# SCADE One → ESP32: A Model-Based Embedded Systems Course

A nine-practice laboratory series that takes a student from a first SCADE One
operator to a **flight-control demonstrator** running on an ESP32 under FreeRTOS,
built on one principle:

> **The model owns the logic. The platform owns the I/O.**

Every practice generates C code from a SCADE One model and wraps it in a thin,
hand-written *glue layer* on the ESP32, without editing the generated code.
Across the series the glue grows from three blinking LEDs into a
priority-scheduled FreeRTOS application that drives four control surfaces, a
landing gear, panel LEDs and telemetry, with an emergency mode built into the model.

**Author:** Leonardo Franco García · Universidad Aeronáutica en Querétaro (UNAQ)
**Stack:** SCADE One (Ansys) with the Swan code generator · ESP-IDF v5.5 · FreeRTOS · ESP32

<!-- DEMO VIDEO / GIF (30–60 s): phases, landing gear, emergency
     Example:
     ![Demo](docs/demo.gif)
-->

---

## The final system (Practice 9)

A benchtop flight-control demonstrator: one generated SCADE model running inside
four FreeRTOS tasks.

- **Flight-phase state machine** (in the model): `GROUND → TAKEOFF → CRUISE → LANDING`,
  selected by the `mode` potentiometer.
- **Control-surface law** (in the model): each phase × direction (left / centre /
  right) commands the left/right ailerons, rudder and elevator.
- **Landing gear** (in the model): down in GROUND, TAKEOFF and LANDING; up in CRUISE.
- **Emergency mode** (in the model): an out-of-range or suddenly jumping `mode`
  input forces all surfaces to neutral (90°) and flashes the red LED. The model
  returns to GROUND once the input has been valid for about 1 s.
- **Task watchdog:** the control task is subscribed to the ESP-IDF Task Watchdog
  and resets it every cycle (see [Watchdog behaviour](#watchdog-behaviour)).

<!-- SCADE ONE MODEL SCREENSHOT
     ![SCADE One model](docs/scade_model.png)
-->

### FreeRTOS tasks

Source: [`Practices/09-integration/lg_aircraft/main/main.c`](Practices/09-integration/lg_aircraft/main/main.c)

| Task    | Priority | Stack (bytes) | Timing                          | Responsibility                                                              |
|---------|:--------:|:-------------:|---------------------------------|-----------------------------------------------------------------------------|
| `model` | 4        | 4096          | 100 ms, `vTaskDelayUntil`       | Steps the generated model, drives panel LEDs, sends servo commands, resets the watchdog |
| `act`   | 3        | 3072          | Blocks on the command queue     | Writes the five servo angles to the LEDC channels                           |
| `sens`  | 3        | 3072          | 50 ms, `vTaskDelay`             | Samples the `mode` and `dir` potentiometers (ADC1 one-shot)                 |
| `tlm`   | 2        | 3072          | 100 ms poll                     | Logs one line on every phase or emergency change                            |

Inter-task communication:

- **Queue `q_act`** (length 4, `cmd_t` = five servo angles): `model` → `act`.
- **Shared variables** `pilot`, `g_phase`, `g_emerg`: `sens` → `model` and `model` → `tlm`.
- **Mutex `logmux`:** guards the telemetry log output.

### Wiring

<!-- WIRING DIAGRAM
     ![Wiring](docs/wiring.png)
-->

| Signal              | Role                    | Pin              |
|---------------------|-------------------------|------------------|
| `mode`              | flight-phase pot (ADC1) | GPIO34           |
| `dir`               | direction pot (ADC1)    | GPIO35           |
| `aleizq` / `aleder` | left / right aileron    | GPIO13 / GPIO12  |
| `tdir` / `elev`     | rudder / elevator       | GPIO14 / GPIO27  |
| `lgr`               | landing gear            | GPIO4            |
| red / green / white | panel LEDs              | GPIO25 / 26 / 33 |

> **Power:** five SG90 servos need an external 5 V supply (≥ 2 A) whose ground is
> tied to the ESP32 ground. Do not power servos from the 3V3 pin.

### Watchdog behaviour

`model_task` subscribes itself with `esp_task_wdt_add(NULL)` and calls
`esp_task_wdt_reset()` at the top of every 100 ms cycle. The Task Watchdog also
monitors both IDLE tasks. With the committed `sdkconfig` (timeout 5 s,
`CONFIG_ESP_TASK_WDT_PANIC` not set), a missed reset makes the watchdog **print
the offending task(s) and a backtrace; it does not reboot the chip**. To make a
hang trigger a panic and reboot, set `CONFIG_ESP_TASK_WDT_PANIC=y`
(`idf.py menuconfig` → Component config → ESP System Settings).

---

## The learning arc

| # | Practice                    | Project folder                                   | What you build                               | Key concept                                     |
|---|-----------------------------|--------------------------------------------------|----------------------------------------------|-------------------------------------------------|
| 1 | SCADE One Fundamentals      | — (model only)                                   | First operators (counter, saturation)        | Synchronous dataflow, `pre`, simulation         |
| 2 | ESP32 Environment Setup     | `02-esp32-setup/lg_env_check`                    | Toolchain + first blink                      | ESP-IDF project anatomy, `idf.py`, `ESP_LOG`    |
| 3 | First Bridge: SCADE → ESP32 | `03-first-bridge/scade_counter`, `lg_bridge`     | Counter over UART, traffic light on 3 LEDs   | Code generation, `reset`/`step`, the glue layer |
| 4 | Framed UART Telemetry       | `04-uart-telemetry/lg_telemetry`                 | Binary telemetry frame + loopback            | Framing, checksums, `esp_driver_uart`           |
| 5 | The Model as a Task         | `05-model-as-task/lg_rtos`                       | Model inside a FreeRTOS task                 | Tasks, queues, mutex, `vTaskDelayUntil`         |
| 6 | Flight-Phase State Machine  | `06-flight-phase/lg_phase`                       | Ground→Takeoff→Cruise→Landing automaton      | SCADE state machines, guarded transitions       |
| 7 | Sensors & Actuators         | `07-landing-gear/lg_gear`                        | Phase-commanded landing gear                 | ADC (potentiometer) + PWM servo (LEDC)          |
| 8 | Fail Safe                   | `08-fail-safe/lg_safe`                           | Fault detection + emergency state + watchdog | Fault detection vs. reaction, `esp_task_wdt`    |
| 9 | Integration Project         | `09-integration/lg_aircraft`                     | Full aircraft: 4 surfaces + gear + emergency | Multi-task RTOS integration                     |

Each manual (PDF) is a self-contained lab session: theory, a worked example, a
main activity, an assessment questionnaire and a troubleshooting section drawn
from real development.

---

## Repository structure

```
.
├── README.md
├── LICENSE
└── Practices/
    ├── 01-fundamentals/
    │   └── Practice-01-SCADE-Fundamentals.pdf
    ├── 02-esp32-setup/
    │   ├── Practice-02-ESP32-Setup.pdf
    │   └── lg_env_check/            # ESP-IDF project
    ├── 03-first-bridge/
    │   ├── Practice-03-First-Bridge.pdf
    │   ├── scade_counter/
    │   └── lg_bridge/
    ├── ...
    └── 09-integration/
        ├── Practice-09-Integration.pdf
        └── lg_aircraft/
            ├── CMakeLists.txt
            ├── sdkconfig
            ├── main/main.c          # glue layer: tasks, queue, mutex, watchdog
            └── components/scade_gen/  # generated C + hand-written swan_config.h
```

---

## How to build

With ESP-IDF v5.x installed and exported:

```
cd Practices/09-integration/lg_aircraft
idf.py set-target esp32
idf.py -p COM8 flash monitor      # replace COM8 with your port
```

Each project's `components/scade_gen/` holds the SCADE-generated C plus the
hand-written `swan_config.h` platform-adaptation header. To change behaviour,
edit the SCADE model, regenerate, and copy the new code in. The generated files
are never edited by hand.

---

## Requirements

### Software
- **SCADE One** (Ansys) with the Swan code generator (student edition is sufficient)
- **ESP-IDF v5.x** (tested with v5.5.4)
- A serial terminal (`idf.py monitor` is enough)

### Hardware

| # | Item                            | Qty   | Used in | Notes                                |
|---|---------------------------------|-------|---------|--------------------------------------|
| 1 | ESP32 DevKit V1                 | 1     | all     | USB data cable (not charge-only)     |
| 2 | Breadboard                      | 1–2   | P3+     | two help for P9                      |
| 3 | Jumper wires (M–M, M–F)         | ~20   | P3+     | signal, power and ground             |
| 4 | LEDs (red, yellow/white, green) | 3     | P3+     | phase / panel indicators             |
| 5 | Resistors 220 Ω                 | 3     | P3+     | LED current limiting                 |
| 6 | Jumper (loopback)               | 1     | P4, P5  | UART2 loopback GPIO4 ↔ GPIO5         |
| 7 | SG90 micro servo                | 1 → 5 | P7 → P9 | 1 in P7, 5 in P9                     |
| 8 | Potentiometer 10 kΩ             | 1 → 2 | P7 → P9 | analog inputs on ADC1                |
| 9 | External 5 V supply (≥ 2 A)     | 1     | P7+     | for the servos, **not** the 3V3 pin  |

---

## Known limitations and errata

This is an educational prototype. The following issues are known and documented
rather than hidden:

**Practice 9 model**
- Emergency detection monitors only the `mode` input; a fault on `dir` is not detected.
- The out-of-range test (`mode < 20` or `mode > 4075`) also fires when the pot is
  turned fully to either end stop, because the pot ends are wired directly to 3V3/GND.
- In the flight-phase automaton the `mode` threshold transitions are evaluated
  before the emergency transition, so a very fast `mode` change can leave the
  surface automaton latched in its emergency (neutral) state while the phase
  automaton continues normally.
- The green and white panel outputs are driven by the same signal as the red one.

**Practice 9 glue**
- The telemetry task reports the instantaneous `emergency` flag, not the latched
  emergency state.
- `logmux` currently has a single user (the telemetry task).

**Manuals**
- Practice 9 describes five tasks, including a separate `safety_task`; the
  implementation uses four tasks, and `model_task` itself resets the watchdog.
- Practice 9 lists representative argument names; the real step signature is in
  `components/scade_gen/operator0_module0.h` (e.g. the gear output is `lgr`).
- Practices 5, 8 and 9 state that the Task Watchdog resets the chip; with the
  committed `sdkconfig` it only reports (see [Watchdog behaviour](#watchdog-behaviour)).
- Landing-gear angle convention: Practices 7–8 use 0° = down, Practice 9 uses 90° = down.
- Practice 4, expected output: the checksums for ticks 50 and 70 should read
  `9A` and `E8`.
- Practice 5 states a 1 ms tick; the committed `sdkconfig` uses `CONFIG_FREERTOS_HZ=100` (10 ms).
- Practices 1–2 refer to the code generator as KCG; SCADE One's generator is the Swan code generator.

---

## License

See [`LICENSE`](LICENSE). The laboratory manuals (text and figures) are shared
for educational use; the code is provided as-is for learning.

---

*Developed as an embedded-systems laboratory series at UNAQ. An educational
prototype that applies model-based, safety-aware design practices; it is not a
certified avionics system.*
