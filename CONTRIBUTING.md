# Contributing to Market Application

Thank you for your interest in contributing to the Market Application! This document provides guidelines and instructions for contributing to the project.

## Code of Conduct

Please be respectful and considerate of others when contributing to this project.

## Development Process

1. Fork the repository
2. Create a new branch for your feature/fix
3. Make your changes
4. Run tests and ensure they pass
5. Submit a pull request

## Coding Standards

### C++ Style Guide

- Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions small and focused
- Use smart pointers for memory management

### File Organization

- Header files go in `include/`
- Implementation files go in `src/`
- Tests go in `tests/`
- Documentation goes in `docs/`

### Naming Conventions

- Classes: PascalCase
- Functions: camelCase
- Variables: camelCase
- Constants: UPPER_CASE
- Namespaces: lowercase

## Testing

- Write unit tests for new features
- Ensure all tests pass before submitting
- Maintain or improve test coverage

## Documentation

- Update documentation for new features
- Add comments for complex code
- Update README.md if necessary
- Add or update architecture diagrams

## Pull Request Process

1. Update the README.md with details of changes if needed
2. Update the documentation with any new features
3. The PR will be merged once you have the sign-off of at least one other developer

## Version Control

- Use meaningful commit messages
- Keep commits focused and atomic
- Reference issues in commit messages

## Building and Testing

```bash
mkdir build
cd build
cmake ..
make
make test
```

## Questions?

Feel free to open an issue for any questions or concerns. 