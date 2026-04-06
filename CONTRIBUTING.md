# Contributing to Shirota: Blood Polarity Ultimate

Thank you for your interest in contributing to this project! We welcome contributions from the community.

## How to Contribute

### Reporting Bugs
1. Check the [Issues](https://github.com/yourusername/ShirotaBloodPolarity/issues) page to see if the bug has already been reported
2. If not, create a new issue with:
   - Clear title describing the bug
   - Detailed description of the issue
   - Steps to reproduce
   - Expected vs actual behavior
   - System information (OS, hardware specs)
   - Screenshots if applicable

### Suggesting Features
1. Check existing issues for similar suggestions
2. Create a new issue with the `enhancement` label
3. Describe the feature in detail
4. Explain why it would be useful
5. Provide mockups or examples if possible

### Code Contributions

#### Setting Up Development Environment
1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/ShirotaBloodPolarity.git`
3. Create a feature branch: `git checkout -b feature/your-feature-name`
4. Set up dependencies (see Building section in README)

#### Development Guidelines
- Follow the existing code style
- Use meaningful variable and function names
- Add comments for complex logic
- Test your changes thoroughly
- Update documentation if needed

#### Submitting Changes
1. Ensure your code compiles without errors
2. Run any existing tests
3. Commit your changes: `git commit -m "Add feature: description"`
4. Push to your branch: `git push origin feature/your-feature-name`
5. Create a Pull Request with:
   - Clear title
   - Detailed description
   - Screenshots/videos if UI changes
   - Reference to any related issues

### Adding New Characters
When adding new characters:
1. Create character data in `Constants.h`
2. Implement unique abilities in the Fighter classes
3. Add character art (if using external assets)
4. Update character selection menu
5. Test balance and gameplay

### Adding New Levels
When adding new levels:
1. Define level data in `Constants.h`
2. Implement level-specific logic in `Level.cpp`
3. Create background generation code
4. Add environmental effects
5. Test performance and gameplay

## Code Style Guidelines

### C++ Standards
- Use C++17 features
- Follow SFML coding conventions
- Use smart pointers where appropriate
- Avoid global variables

### Naming Conventions
- Classes: `PascalCase`
- Functions: `camelCase`
- Variables: `camelCase`
- Constants: `UPPER_SNAKE_CASE`

### File Structure
- Headers in `include/`
- Sources in `src/`
- Resources in `resources/`
- One class per file (header + implementation)

## Testing
- Test on multiple Windows versions
- Verify with different hardware configurations
- Check memory usage and performance
- Test edge cases and error conditions

## Documentation
- Update README.md for major changes
- Add comments for new features
- Update changelog
- Provide usage examples

## Community Guidelines
- Be respectful and constructive
- Help newcomers learn
- Follow the code of conduct
- Give credit where due

## Recognition
Contributors will be acknowledged in the README and potentially in-game credits.

Thank you for helping make Shirota: Blood Polarity Ultimate even better! 🎮✨