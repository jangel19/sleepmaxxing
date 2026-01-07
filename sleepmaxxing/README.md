# Sleepmaxxing

Sleepmaxxing is a **C++ analytical engine** for detecting **recovery trends** and **circadian rhythm shifts** using exported wearable data (e.g., Garmin, WHOOP, Amazfit).

The project is intentionally designed to be:
- Deterministic
- Explainable
- Robust to missing data
- Free of black-box machine learning

All insights are derived **strictly from user-provided data**.

---

## Motivation

Sleepmaxxing started as a personal project during a university finals period where my sleep quality significantly declined.
Despite wearing a Garmin device and tracking metrics such as sleep duration, HRV, and resting heart rate, the platform’s summaries did not clearly explain **how much this change in sleep was affecting my recovery over time**.

This project was built to answer questions such as:
- How does sustained short sleep affect recovery trends?
- Are changes gradual or abrupt?
- Are observed declines meaningful or just day-to-day noise?

Rather than relying on opaque “readiness scores,” Sleepmaxxing focuses on **explicit comparisons between recent behavior and a short-term baseline**, using transparent and explainable logic.

---

## Data Input

Sleepmaxxing operates on **CSV exports** provided directly by the user.

Typical sources include:
- Garmin health data exports
- WHOOP data exports
- Other wearable platforms that provide daily summaries

The engine does not collect, store, or fetch data automatically.
Users retain full control over what data is analyzed.

---

## Core Design (v1)

### Time Windowing

Analysis is based on **two adjacent time windows**:
- **Baseline window:** 14 days
- **Recent window:** 14 days

The engine compares recent behavior against the immediately preceding baseline to detect **short-term changes** while minimizing seasonal and long-term drift.

A minimum of **5 valid days per window** is required for any metric to be considered.

---

### Recovery Trend Analysis

Recovery trends are computed using the following metrics when available:
- Heart Rate Variability (HRV)
- Resting Heart Rate (RHR)
- Stress

Each metric is analyzed independently using:
- Median (robust center)
- Median Absolute Deviation (robust scale)

Metrics contribute standardized effect sizes that are combined to determine whether recovery is:
- Improving
- Stable
- Declining

All contributions are explainable and directly traceable to observed data.

---

### Circadian Shift Detection

Wake time is treated as a **circular variable** on a 24-hour clock.

The engine:
- Converts wake times to angles
- Computes circular means
- Measures the shortest phase difference between windows

Additional safeguards include:
- Minimum data requirements
- Stability checks using circular resultant magnitude

The result indicates whether wake time has shifted earlier or later, and whether the shift is significant.

---

## Output

The engine produces:
- Recovery trend classification
- Circadian shift (in hours)
- Confidence indicators
- Supporting statistics for transparency

All outputs are deterministic and reproducible.

---

## Architecture

The system is modular and intentionally layered:

- CSV ingestion and validation
- Robust statistics
- Windowed comparisons
- Trend inference
- Final result aggregation

Core analysis logic is written in **modern C++ (C++17+)**.

---

## What Sleepmaxxing Is Not

- Not a medical device
- Not a diagnostic tool
- Not a machine-learning model
- Not a real-time tracker

This project focuses on **explainable trend analysis**, not prediction or prescription.

---

## Planned Extensions (v2+)

Potential future improvements include:
- User-selectable comparison windows
- Custom baseline date selection within a CSV
- Support for additional wearable export formats
- Optional CLI interface
- Optional WebAssembly build for browser-based demos
- Optional direct API integrations (e.g., Garmin API), if access permits

The current v1 intentionally favors strong defaults and minimal configuration.

---

## License

Open source.
---

## Disclaimer

Sleepmaxxing provides observational insights based on historical data only.
It is not intended to diagnose, treat, or prevent any medical condition.
