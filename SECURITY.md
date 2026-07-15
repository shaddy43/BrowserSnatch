# Security Policy

BrowserSnatch is an offensive-security research tool intended **exclusively** for authorized penetration testing and education. This document explains how to report vulnerabilities **in the tool itself** and sets expectations for responsible use.

> Using this tool against systems, accounts, or data without explicit written authorization may be illegal. See [ACCEPTABLE_USE.md](ACCEPTABLE_USE.md). This policy does not grant any testing authorization.

---

## Reporting a Vulnerability

If you discover a security issue **in BrowserSnatch's own code** (for example, a memory-safety bug, a supply-chain concern, or unintended behavior), please report it privately.

**Preferred:** Use GitHub's private vulnerability reporting, go to the repository's **Security → Report a vulnerability** ("Report a vulnerability" under the Security Advisories tab).

**Alternative:** Contact the author directly via [https://shaddy43.github.io](https://shaddy43.github.io).

Please do **not** open a public GitHub Issue for security-sensitive reports.

### What to include

- A clear description of the issue and its impact.
- Steps to reproduce (proof-of-concept welcome).
- Affected version / commit hash.
- Your suggested remediation, if any.

### What to expect

- **Acknowledgment:** within 5 business days.
- **Assessment & triage:** within 15 business days.
- **Fix / disclosure:** coordinated with you; we aim to resolve valid issues promptly and will credit reporters who wish to be named.

Please give us reasonable time to remediate before any public disclosure.

---

## Scope

**In scope**
- Bugs and vulnerabilities in the BrowserSnatch source code and build.
- Issues in this repository's documentation that could mislead users into unsafe or unlawful behavior.
- Detections and rules for the tool to limit Evasion

**Out of scope**
- Reports that BrowserSnatch is "malicious" or "a stealer", this is its documented dual-use nature as a security-research tool. Such reports will be closed.
- Requests for help using the tool against third parties, or against targets you are not authorized to test. These will be declined and may be reported.
- Extending the applications of BrowserSnatch to other sensitive topics (steam, cypto, banking apps etc). This will be declined.
- Vulnerabilities in the browsers themselves - report those to the respective browser vendors.

---

## Supported Versions

Security fixes are applied to the latest version on the `master` branch. Older builds are not maintained; please update before reporting.

| Version | Supported |
| --- | --- |
| Latest `master` | ✅ |
| Older releases | ❌ |

---

## Responsible & Legal Use

This project is published to help defenders understand and counter real-world credential-theft techniques. Everyone interacting with this repository is expected to:

- Operate **only** within explicit, written authorization and defined rules of engagement.
- Comply with all applicable laws (e.g. the U.S. Computer Fraud and Abuse Act, the UK Computer Misuse Act, the EU Directive 2013/40/EU, and local equivalents).
- Handle any data encountered confidentially and destroy it securely when the engagement ends.

Full terms are in [ACCEPTABLE_USE.md](ACCEPTABLE_USE.md). The author accepts no liability for misuse.
