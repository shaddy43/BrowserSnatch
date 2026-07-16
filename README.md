# BrowserSnatch

<div align="center">
  <img width="500px" src="Assets/browser_snatch1.jpg" />
</div>

### A Versatile Browser Data Extraction Tool for Authorized Security Testing

**BrowserSnatch** is an offensive-security research tool that extracts and decrypts sensitive data from web browsers. It is built for **red teams, penetration testers, and security researchers** to demonstrate the real-world impact of endpoint compromise during **authorized engagements**. It targets both Chromium-based browsers (Chrome, Edge, Brave, Opera, etc.) and Gecko-based browsers (Firefox, Thunderbird, etc.), covering **40+ browsers**.

> [!WARNING]
> **This tool is for LEGAL, AUTHORIZED penetration testing ONLY.**
> Running BrowserSnatch against systems, accounts, or data you do not own or lack **explicit written permission** to test may be a **crime** in your jurisdiction (e.g. the U.S. Computer Fraud and Abuse Act, the UK Computer Misuse Act, and equivalent laws worldwide). You are solely responsible for how you use it. See [Acceptable Use](ACCEPTABLE_USE.md) before you build or run anything.

---

## ✅ Authorized Use Only — Read This First

By downloading, building, or running BrowserSnatch, you confirm that:

- You are using it **only** on systems you own, or for which you hold **explicit, written authorization** (e.g. a signed penetration-testing agreement, rules of engagement, or scope document).
- Your use complies with **all** applicable local, national, and international laws.
- You will **not** use it to access, exfiltrate, or handle data belonging to others without consent.
- You accept **full and sole responsibility** for your actions. The author provides this software for defensive research and education and is **not liable** for any misuse.

If you cannot agree to all of the above, **do not use this software.**

Full terms: [ACCEPTABLE_USE.md](ACCEPTABLE_USE.md) · Vulnerability reporting: [SECURITY.md](SECURITY.md) · License: [LICENSE](LICENSE)

---

## 📄 BrowserSnatch Wiki — Complete Detection Guide

BrowserSnatch is intended for authorized penetration testing but like any dual-use offensive software, it is being abused.

This wiki gives blue teams, threat hunters, and detection engineers a **ready-to-deploy** detection package: Sigma rules, YARA rules, Detection Queries and Hunts, IOCs, and MITRE ATT&CK mappings, all in one browsable place.

[BrowserSnatch Detection & Threat Intelligence](https://github.com/shaddy43/BrowserSnatch/wiki)

---

## 🎯 Why This Exists

Credential and session theft from browsers is one of the most common techniques used by real-world malware and adversaries. BrowserSnatch reproduces those techniques in a controlled, inspectable, open-source form so that:

- **Red teams** can accurately model post-exploitation impact during sanctioned engagements.
- **Blue teams and defenders** can study how browser data is stored, encrypted, and extracted in order to build better detections and hardening.
- **Researchers and students** can learn how modern browser encryption (including app-bound encryption) works and where it can fail.

Understanding the offense is a prerequisite for effective defense.

---

## 🚀 Capabilities

- **App-Bound Encrypted Data (v20)**: Extract and decrypt the latest app-bound encrypted data (Chrome, Edge, Brave).
- **Saved Passwords**: Retrieve stored passwords from major Chromium and Gecko-based browsers.
- **Cookies**: Extract cookies from user profiles across multiple browsers.
- **Bookmarks**: Collect saved bookmarks from every supported browser.
- **History**: Extract browsing history across all supported browsers.
- **Fast**: Written in C/C++ for high performance.
- **Minimal Dependencies**: Written in C/C++ with little to no need for external libraries.
- **Cross-Browser**: Handles both Chromium and Gecko-based browsers.
- **Greed Mode**: Collects everything into a single consolidated database.

---

## 📜 Code of Conduct

By using BrowserSnatch, you agree to:

- Use this tool in compliance with all local, state, national, and international laws.
- Obtain **explicit written authorization** before testing any system, network, or data.
- Stay within the agreed scope / rules of engagement of your assessment.
- Respect privacy and confidentiality when handling any data you encounter.
- Securely destroy extracted data once your authorized engagement concludes.

---

## 🛠️ Build

- Clone the repository.
- Open in Visual Studio.
- Use ISO C++17 (`/std:c++17`) or higher.
- Install any missing packages (e.g. `nlohmann`) via the Visual Studio NuGet installer.
- Compile.

> [!NOTE]
> Building this software does not grant you permission to use it against any particular target. Authorization is your responsibility.

---

## 🛠️ Usage

Run BrowserSnatch from the command line. With no parameter it operates in default mode.

**Default Mode**
- No parameter: attempts to snatch all saved passwords and cookies.

**Command-Line Mode**
- `-h`: Display the help menu with all available options.

**Password Snatching**
- `-pass`: Passwords from every browser.
- `-pass -c`: Chromium-based browsers only.
- `-pass -g`: Gecko-based browsers only.

**Cookie Snatching**
- `-cookies`: Cookies from every browser.
- `-cookies -c`: Chromium-based browsers only.
- `-cookies -g`: Gecko-based browsers only.
- `-app-bound-decryption`: Cookies from the latest v20 app-bound encryption only (requires admin).

**Bookmarks Snatching**
- `-bookmarks`: Bookmarks from every browser.
- `-bookmarks -c`: Chromium-based browsers only.
- `-bookmarks -g`: Gecko-based browsers only.

**History Snatching**
- `-history`: History from every browser.
- `-history -c`: Chromium-based browsers only.
- `-history -g`: Gecko-based browsers only.

**Greed Mode**
- `-greed`: Snatch everything from every browser into a single consolidated database.

> [!NOTE]
> - If cookie or password snatching fails, run `-app-bound-decryption` mode first.
> - If it still fails, run `-recalibrate` mode.

<p align="center">
  <img src="Assets/help-menu.png" alt="Help Menu" width="600"/>
</p>

---

## 🎬 Demo

The following GIF demonstrates BrowserSnatch and how its stealer log can be accessed.

![Demo](Assets/Demo.gif)

---

## 🌐 Supported Browsers

| № | Browser Name | Passwords | Cookies | Bookmarks | History | v20 Cookie Decryption (App-Bound) |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | Chrome | ✅ | ✅ | ✅ | ✅ | ✅ |
| 2 | Microsoft Edge | ✅ | ✅ | ✅ | ✅ | ✅ |
| 3 | Chromium | ✅ | ✅ | ✅ | ✅ | ➖ |
| 4 | Brave Browser | ✅ | ✅ | ✅ | ✅ | ✅ |
| 5 | Epic Privacy Browser | ✅ | ✅ | ✅ | ✅ | ➖ |
| 6 | Amigo | ✅ | ✅ | ✅ | ✅ | ➖ |
| 7 | Vivaldi | ✅ | ✅ | ✅ | ✅ | ➖ |
| 8 | Orbitum | ✅ | ✅ | ✅ | ✅ | ➖ |
| 9 | SeaMonkey | ✅ | ✅ | ✅ | ✅ | ➖ |
| 10 | Kometa | ✅ | ✅ | ✅ | ✅ | ➖ |
| 11 | Comodo Dragon | ✅ | ✅ | ✅ | ✅ | ➖ |
| 12 | Torch | ✅ | ✅ | ✅ | ✅ | ➖ |
| 13 | Icecat | ✅ | ✅ | ✅ | ✅ | ➖ |
| 14 | Postbox | ✅ | ✅ | ✅ | ✅ | ➖ |
| 15 | Flock Browser | ✅ | ✅ | ✅ | ✅ | ➖ |
| 16 | K-Melon | ✅ | ✅ | ✅ | ✅ | ➖ |
| 17 | Sputnik | ✅ | ✅ | ✅ | ✅ | ➖ |
| 18 | CocCoc Browser | ✅ | ✅ | ✅ | ✅ | ➖ |
| 19 | Uran | ✅ | ✅ | ✅ | ✅ | ➖ |
| 20 | Yandex | ✅ | ✅ | ✅ | ✅ | ➖ |
| 21 | Firefox | ✅ | ✅ | ✅ | ✅ | ➖ |
| 22 | Waterfox | ✅ | ✅ | ✅ | ✅ | ➖ |
| 23 | Cyberfox | ✅ | ✅ | ✅ | ✅ | ➖ |
| 24 | Thunderbird | ✅ | ✅ | ✅ | ✅ | ➖ |
| 25 | IceDragon | ✅ | ✅ | ✅ | ✅ | ➖ |
| 26 | BlackHawk | ✅ | ✅ | ✅ | ✅ | ➖ |
| 27 | Pale Moon | ✅ | ✅ | ✅ | ✅ | ➖ |
| 28 | Opera | ✅ | ✅ | ✅ | ✅ | ➖ |
| 29 | Iridium | ✅ | ✅ | ✅ | ✅ | ➖ |
| 30 | CentBrowser | ✅ | ✅ | ✅ | ✅ | ➖ |
| 31 | Chedot | ✅ | ✅ | ✅ | ✅ | ➖ |
| 32 | liebao | ✅ | ✅ | ✅ | ✅ | ➖ |
| 33 | 7Star | ✅ | ✅ | ✅ | ✅ | ➖ |
| 34 | ChromePlus | ✅ | ✅ | ✅ | ✅ | ➖ |
| 35 | Citrio | ✅ | ✅ | ✅ | ✅ | ➖ |
| 36 | 360Chrome | ✅ | ✅ | ✅ | ✅ | ➖ |
| 37 | Elements Browser | ✅ | ✅ | ✅ | ✅ | ➖ |
| 38 | Sleipnir5 | ✅ | ✅ | ✅ | ✅ | ➖ |
| 39 | ChromiumViewer | ✅ | ✅ | ✅ | ✅ | ➖ |
| 40 | QIP Surf | ✅ | ✅ | ✅ | ✅ | ➖ |
| 41 | Coowon | ✅ | ✅ | ✅ | ✅ | ➖ |

---

## 🔄 Roadmap

- **Stability**: A more stable, reliable tool.
- **Defensive documentation**: A detection/hardening guide for blue teams (planned).

---

## 📄 License & Terms

- **License**: [MIT](LICENSE) © 2026 Shayan Ahmed Khan (shaddy43)
- **Acceptable Use**: [ACCEPTABLE_USE.md](ACCEPTABLE_USE.md) - required reading; governs how you may use this software.

The MIT license grants broad software rights but **does not** authorize you to access systems or data you do not own or lack permission to test. Legal authorization is separate from, and additional to, the software license.

---

## 📧 Contact

For inquiries or contributions, reach out to the [author](https://shaddy43.github.io) or open a GitHub Issue (non-security topics only).

---

## 🙏 Acknowledgments

- Inspired by **SaulBerrenson**'s [BrowserStealer](https://github.com/SaulBerrenson/BrowserStealer).
- Chrome key & password decryption from [0x00sec](https://0x00sec.org/t/malware-development-1-password-stealers-chrome/33571).
- App-bound encryption key PoC by [snovvcrash](https://gist.github.com/snovvcrash/caded55a318bbefcb6cc9ee30e82f824).
- Additional browser support from [xaitax](https://github.com/xaitax/Chrome-App-Bound-Encryption-Decryption).
