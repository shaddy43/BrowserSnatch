## BrowserSnatch
----------------------------------------------------

<div align="center">
  <img width="500px" src="Assets/browser_snatch.jpg" />
</div>

### A Versatile Browser Data Extraction Tool

BrowserSnatch is a powerful and versatile tool designed to **"snatch"** sensitive data from a variety of web browsers. This tool targets both Chromium-based browsers (such as Edge, Chrome, Opera, etc.) and Gecko-based browsers (such as Firefox, Thunderbird, etc.) overall including more than **40 browsers**, making it a comprehensive solution for browser data extraction.

--------------------------------------------------

### 🚀 Capabilities
- **Snatch Saved Passwords**: Effortlessly retrieve stored passwords from all major Chromium and Gecko-based browsers
- **Snatch Cookies**: Extract cookies from user profiles across multiple browsers
- **Upcoming Features**: Future versions will also support snatching of:

        - Browser History

**Note**: This version does not include obfuscated strings or API calls and any other defense evasion technique. Those are planned for future releases.

--------------------------------------------------

### ✨ Whats NEW?

- **Easy to Use**: No complicated setup or configuration required
- **Zero External Dependencies**: Completely written in C and C++, with little to no need for additional libraries
- **Cross-Browser Support**: Handles both Chromium and Gecko-based browsers with ease
- **Future-Proof**: Continuous updates with more capabilities and advanced evasion techniques

--------------------------------------------------

### ⚠️ Disclaimer

BrowserSnatch is a tool designed for legal, ethical penetration testing and educational purposes only. The author is NOT responsible for any misuse or illegal activities performed using this tool. Always ensure you have proper authorization before testing any system or network.

--------------------------------------------------

### 📜 Code of Conduct

By using BrowserSnatch, you agree to:

- Use this tool in compliance with all local, state, and federal laws
- Obtain proper authorization before testing any system or network
- Respect privacy and confidentiality when handling data

--------------------------------------------------

### 🛠️ Usage
To run BrowserSnatch, simply execute the binary from the command line. The tool will operate in default mode if no parameter is provided.

**Default Mode**
- No Parameter Provided: Executes with default settings and attempts to snatch all saved passwords and cookies

**CommandLine Mode**
- Parameter: -h: Displays a help menu detailing all available options.

  **Password Snatching**
    - Parameter: -pass: Snatch passwords from every browser.
    - Parameter: -pass -c: Snatch passwords from Chromium-based browsers only.
    - Parameter: -pass -g: Snatch passwords from Gecko-based browsers only.

  **Cookie Snatching**
    - Parameter: -cookies: Snatch cookies from every browser.
    - Parameter: -cookies -c: Snatch cookies from Chromium-based browsers only.
    - Parameter: -cookies -g: Snatch cookies from Gecko-based browsers only.

  **Bookmarks Snatching**
    - Parameter: -bookmarks: Snatch bookmarks from every browser.
    - Parameter: -bookmarks -c: Snatch bookmarks from Chromium-based browsers only.
    - Parameter: -bookmarks -g: Snatch bookmarks from Gecko-based browsers only.

<!-- ![Help Menu](Assets/help-menu.png) -->
<p align="center">
  <img src="Assets/help-menu-updated.png" alt="Help Menu" width="600"/>
</p>

**Console Mode**
- Parameter: -console-mode: Displays a user-friendly console.

<!-- ![Help Menu](Assets/console-mode.png) -->
<p align="center">
  <img src="Assets/console-mode-updated.png" alt="Help Menu" width="600"/>
</p>

#### 📝 Example
- Run the following command to start BrowserSnatch in default mode:
```sh
./BrowserSnatch
```

- To see the user-friendly console interface, use:
```sh
./BrowserSnatch -console-mode
```

- To see help menu, use:
```sh
./BrowserSnatch -h
```

- To Snatch all browser passwords, use:
```sh
./BrowserSnatch -pass
```

- To Snatch chromium browser passwords, use:
```sh
./BrowserSnatch -pass -c
```

- To Snatch gecko browser passwords, use:
```sh
./BrowserSnatch -pass -g
```

- To Snatch all browser cookies, use:
```sh
./BrowserSnatch -cookies
```

- To Snatch chromium browser cookies, use:
```sh
./BrowserSnatch -cookies -c
```

- To Snatch gecko browser cookies, use:
```sh
./BrowserSnatch -cookies -g
```

- To Snatch all browser bookmarks, use:
```sh
./BrowserSnatch -bookmarks
```

- To Snatch chromium browser bookmarks, use:
```sh
./BrowserSnatch -bookmarks -c
```

- To Snatch gecko browser bookmarks, use:
```sh
./BrowserSnatch -bookmarks -g
```

--------------------------------------------------

### DEMO
Following GIF demonstrates the working of BrowserSnatch and how its stealer log can be accessed.

![Demo](Assets/Demo.gif)

--------------------------------------------------

### Supported Browser Snatch:

| № | Browser Name | Passwords | Cookies | Bookmarks | History |
| --- | --- | --- | --- | --- | --- |
| 1 | Chrome | &#9989; | &#9989; | &#9989; | &#8987; |
| 2 | Microsoft Edge | &#9989; | &#9989; | &#9989; | &#8987; |
| 3 | Chromium | &#9989; | &#9989; | &#9989; | &#8987; |
| 4 | Brave - Browser | &#9989; | &#9989; | &#9989; | &#8987; |
| 5 | Epic Privacy Browser | &#9989; | &#9989; | &#9989; | &#8987; |
| 6 | Amigo | &#9989; | &#9989; | &#9989; | &#8987; |
| 7 | Vivaldi | &#9989; | &#9989; | &#9989; | &#8987; |
| 8 | Orbitum | &#9989; | &#9989; | &#9989; | &#8987; |
| 9 | SeaMonkey | &#9989; | &#9989; | &#9989; | &#8987; |
| 10 | Kometa | &#9989; | &#9989; | &#9989; | &#8987; |
| 11 | Comodo Dragon | &#9989; | &#9989; | &#9989; | &#8987; |
| 12 | Torch | &#9989; | &#9989; | &#9989; | &#8987; |
| 13 | Icecat | &#9989; | &#9989; | &#9989; | &#8987; |
| 14 | Postbox | &#9989; | &#9989; | &#9989; | &#8987; |
| 15 | Flock Browser | &#9989; | &#9989; | &#9989; | &#8987; |
| 16 | K - Melon | &#9989; | &#9989; | &#9989; | &#8987; |
| 17 | Sputnik | &#9989; | &#9989; | &#9989; | &#8987; |
| 18 | CocCoc Browser | &#9989; | &#9989; | &#9989; | &#8987; |
| 19 | Uran | &#9989; | &#9989; | &#9989; | &#8987; |
| 20 | Yandex | &#9989; | &#9989; | &#9989; | &#8987; |
| 21 | Firefox | &#9989; | &#9989; | &#9989; | &#8987; |
| 22 | Waterfox | &#9989; | &#9989; | &#9989; | &#8987; |
| 23 | Cyberfox | &#9989; | &#9989; | &#9989; | &#8987; |
| 24 | Thunderbird | &#9989; | &#9989; | &#9989; | &#8987; |
| 25 | IceDragon | &#9989; | &#9989; | &#9989; | &#8987; |
| 26 | BlackHawk | &#9989; | &#9989; | &#9989; | &#8987; |
| 27 | Pale Moon | &#9989; | &#9989; | &#9989; | &#8987; |
| 28 | Opera | &#9989; | &#9989; | &#9989; | &#8987; |
| 29 | Iridium | &#9989; | &#9989; | &#9989; | &#8987; |
| 30 | CentBrowser | &#9989; | &#9989; | &#9989; | &#8987; |
| 31 | Chedot | &#9989; | &#9989; | &#9989; | &#8987; |
| 32 | liebao | &#9989; | &#9989; | &#9989; | &#8987; |
| 33 | 7Star | &#9989; | &#9989; | &#9989; | &#8987; |
| 34 | ChromePlus | &#9989; | &#9989; | &#9989; | &#8987; |
| 35 | Citrio | &#9989; | &#9989; | &#9989; | &#8987; |
| 36 | 360Chrome - Chrome | &#9989; | &#9989; | &#9989; | &#8987; |
| 37 | Elements Browser | &#9989; | &#9989; | &#9989; | &#8987; |
| 38 | Sleipnir5 | &#9989; | &#9989; | &#9989; | &#8987; |
| 39 | ChromiumViewer | &#9989; | &#9989; | &#9989; | &#8987; |
| 40 | QIP Surf | &#9989; | &#9989; | &#9989; | &#8987; |
| 41 | Coowon| &#9989; | &#9989; | &#9989; | &#8987; |

--------------------------------------------------

### 🔄 Future Updates
- **Bookmark and History Snatching**: Currently under development
- **Advanced Defense Evasion Techniques**: String obfuscation, dynamic api resolution and other defense evasion techniques

Stay tuned for future releases!

--------------------------------------------------

### 📧 Contact
For any inquiries or contributions, feel free to reach out to the [author](https://shaddy43.github.io) or contribute directly via GitHub Issues.

--------------------------------------------------

### 🙏 Acknowledgments
Took a lot of help from the Project by **SaulBerrenson** called [**BrowserStealer**](https://github.com/SaulBerrenson/BrowserStealer).
Took help with chrome key decryption from [**0x00sec**](https://0x00sec.org/t/malware-development-1-password-stealers-chrome/33571).
