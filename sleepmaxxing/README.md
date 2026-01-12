# Sleepmaxxing
**Local-first recovery intelligence built in C++**

Sleepmaxxing is a **local, explainable recovery analysis engine** that learns from your personal health data over time.  
It analyzes sleep, HRV, resting heart rate, stress, and activity to generate:

- Weekly recovery summaries
- A recovery score
- Short-term HRV predictions
- Actionable, human-readable guidance
- Model reliability metrics that improve as the system learns

No cloud. No subscriptions. No external user data.  
Everything runs **locally on your machine**.

---

## What This Project Does

Sleepmaxxing is designed to simulate how a real user would use a recovery product:

1. You log daily health data in a CSV
2. The system collects a baseline (first few weeks)
3. It begins making **weekly predictions**
4. Each prediction is evaluated against real outcomes
5. The model adapts and becomes more reliable over time

After ~5 evaluated weeks, you start seeing **accuracy metrics** instead of placeholders.

---

## Demo

An 8-week end-to-end demo (compile → run → prediction → explanation) is included here:  
https://www.loom.com/share/05b3960518ab4bc0ae42a1ede4591d17

---

## Input Data Format

Sleepmaxxing reads a CSV file with **one row per day**.

### CSV Columns

```text
date,sleep_min,wake_min,hrv,rhr,stress,activity
```
---
## Column Definitions

| Column     | Meaning |
|------------|---------|
| `date`     | Calendar day of the record (YYYY-MM-DD) |
| `sleep_min` | Difference from your *normal* sleep duration (minutes) |
| `wake_min` | Difference from your *normal* wake time (minutes) |
| `hrv`      | Heart Rate Variability (milliseconds) |
| `rhr`      | Resting Heart Rate (beats per minute) |
| `stress`  | Daily stress score (1 = low, 5 = high) |
| `activity` | Total steps for the day |

---

### Important

- `sleep_min` and `wake_min` are **relative values**, not raw timestamps
- This allows the system to reason about consistency and deviation, not absolute schedules.

## Example Row

```csv
2025-01-01,10,-5,55,65,3,6500
```

###Interpretation

- Slept 10 minutes more than usual
- Woke up 5 minutes earlier than usual
- HRV = 55 ms
- RHR = 65 bpm
- Moderate stress
- 6,500 steps

---

## How to Build & Run

### 1. Dependencies

#### C++ Dependencies

You must install:

- **mlpack (C++)**
- **Armadillo (C++)**

##### On macOS (Homebrew)

```bash
brew install mlpack armadillo
```
#### On Linux
```bash
sudo apt install libmlpack-dev libarmadillo-dev
```
---
## 2. Build the Project

From the project root:

```bash
make clean
make
```
This builds the sleepmaxxing binary.

## 3. Run Sleepmaxxing

```bash
./sleepmaxxing data/myhealth.csv YYYY-MM-DD
```
###Example
```bash
./sleepmaxxing data/myhealth.csv 2025-02-25
```
The date must exist in the CSV and represents the “current week” the system should analyze.
---
## What You’ll See

A typical run outputs:

- **Recovery Summary** (Improving / Stable / Declining)
- **Recovery Score** (0–100)
- **Confidence level**
- **Predicted HRV change** (7-day horizon)
- **Plain-English interpretation**
- **Primary physiological drivers**
- **Actionable guidance**
- **Model learning & reliability stats**

---

### Example

```text
RECOVERY SCORE FOR THIS WEEK: 80
Strong Recovery

PREDICTED HRV DELTA (next 7 days): +1.11 ms
```

---
## Model Reliability Explained

Sleepmaxxing is **intentionally conservative** with metrics.

- Predictions are evaluated **only when real outcomes exist**
- Accuracy metrics appear **after enough evaluated predictions**
- Early weeks prioritize **transparency over false confidence**
### Typical progression

- **Weeks 1–3** → Baseline collection  
- **Weeks 4–5** → Predictions + evaluation  
- **Week 5+** → Directional accuracy appears and improves
---
## Features (V1)

- **Local-only C++ recovery engine**
- **Weekly HRV prediction** (ridge regression via mlpack)
- **Recovery score** (0–100)
- **Driver attribution** (sleep, HRV, RHR, stress)
- **Confidence-aware explanations**
- **Incremental learning from past errors**
- **CSV-based data ingestion**
- **Clean Makefile-based build**

---
## Planned V2 Improvements

- **Day-to-day predictions** (not just weekly)

- **Richer activity modeling**
  - Currently: steps only
  - Planned: intensity, training load, recovery days

- **More accurate ML models**
  - Feature scaling
  - Better regularization
  - Per-user adaptation

- **Improved recovery score calibration**

- **Graphs & visualization**
  - Trends
  - Prediction vs outcome
  - HRV trajectories

**Graph integration coming soon**
---
## Philosophy

Sleepmaxxing is built around a simple idea:

> Personal health models should be **local**, **explainable**, and **earned over time**.

No black boxes.  
No dashboards hiding uncertainty.  
No cloud dependency.
---
## Author

Built by **Jordi Lopez**  
Early-stage systems + ML project focused on recovery intelligence.

If you’re interested in health tech, systems programming, or applied ML, feel free to reach out via my website:

 **https://jordisworld.me**
