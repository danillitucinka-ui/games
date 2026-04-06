# Shirota: Blood Polarity Ultimate - GitHub Actions Build

This repository includes automated building via GitHub Actions.

## How it works:
1. **Trigger**: Builds on every push to `main`/`master` and pull requests
2. **Platform**: Windows with MinGW
3. **Dependencies**: Downloads SFML 2.5.1 automatically
4. **Build Process**:
   - CMake configuration
   - MinGW compilation
   - Copy required DLLs
5. **Artifacts**: Uploads built executable and DLLs
6. **Releases**: Creates automatic releases on main branch pushes

## Files included:
- `.github/workflows/build.yml` - Main build workflow
- `README.md` - Project documentation
- `LICENSE` - MIT License
- `CHANGELOG.md` - Version history
- `CONTRIBUTING.md` - Contribution guidelines
- `CODE_OF_CONDUCT.md` - Community standards
- `.github/ISSUE_TEMPLATE/` - Issue templates
- `.github/pull_request_template.md` - PR template

## Build Status
[![Build Status](https://github.com/yourusername/ShirotaBloodPolarity/workflows/Build%20Shirota%20Blood%20Polarity/badge.svg)](https://github.com/yourusername/ShirotaBloodPolarity/actions)

## Download Latest Release
[Download ShirotaBloodPolarity.exe](https://github.com/yourusername/ShirotaBloodPolarity/releases/latest)

The workflow ensures that every code change is automatically tested and built, providing users with fresh releases and contributors with immediate feedback.