# Contributing to RayPals

Thank you for considering contributing to RayPals! This document outlines the process for contributing to the project.

## Code of Conduct

Please be respectful and considerate of others when contributing to this project. We appreciate your help in maintaining a positive and inclusive environment.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue on GitHub with the following information:
- A clear, descriptive title
- A detailed description of the bug
- Steps to reproduce the bug
- Expected behavior vs. actual behavior
- Screenshots (if applicable)
- Your environment details (OS, compiler, raylib version, etc.)

### Suggesting Features

We welcome feature suggestions! Please create an issue on GitHub with:
- A clear, descriptive title
- A detailed description of the feature you'd like to see
- Any relevant examples, sketches, or mockups
- Explanation of how this feature would benefit the project

### Pull Requests

We actively welcome pull requests:

1. Fork the repository
2. Create a new branch for your feature or bugfix
3. Make your changes
4. Run tests to ensure your changes don't break existing functionality
5. Submit a pull request

For significant changes, please open an issue first to discuss what you would like to change.

## Development Setup

1. Clone the repository:
```bash
git clone https://github.com/yourusername/raypals.git
cd raypals
```

2. Create a build directory:
```bash
mkdir build && cd build
```

3. Configure and build:
```bash
cmake ..
cmake --build .
```

4. Run tests:
```bash
ctest
```

## Coding Style

Please follow these guidelines:
- Use consistent indentation (4 spaces)
- Use clear, descriptive variable and function names
- Add comments for complex logic
- Follow the existing code style in the project

## Documentation

- Document all public functions with clear, concise comments
- Include examples where appropriate
- Update the README.md if necessary

## License

By contributing to RayPals, you agree that your contributions will be licensed under the project's MIT license. 