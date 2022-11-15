\ifndef{skipblock}example of a minimal possible program.\endif
---
title: Hello World (hello)
copyright: © 2021 AO Kaspersky Lab
include-after: Information about third-party code is contained in the file legal_notices.txt, in the application installation folder.
---

Example of a minimal program for KasperskyOS that prints "Hello World!" to `stderr`.

## Init description

To print to `stderr` using libc library functions, it is sufficient to specify the `hello` entity for it to be started.

<details><summary>einit/src/init.yaml.in</summary>
```yaml
\include{"einit/src/init.yaml.in"}
```
</details>


## Security configuration

The security configuration in this example allows you to run all entities and interaction with the `Core` entity.

<details><summary>einit/src/security.psl.in</summary>
```c
\include{"einit/src/security.psl.in"}
```
</details>


## Implementation of the `Hello` entity

The `Hello` entity prints "Hello World!" to `stderr`.

<details><summary>hello/src/hello.c</summary>
```c
\include{"hello/src/hello.c"}
```
</details>


## Build script

The CMake build system is used in this example.

EDL description of the `Hello` entity is generated directly in CMake scripts using the `nk_build_edl_files` function provided by NK.

*./hello/CMakeLists.txt* contains the build instructions for the `Hello` entity:

<details><summary>hello/CMakeLists.txt</summary>
```cmake
\include{"hello/CMakeLists.txt"}
```
</details>

*./einit/CMakeLists.txt* contains the build instructions for the `Einit` entity and the system image.

<details><summary>einit/CMakeLists.txt</summary>
```cmake
\include{"einit/CMakeLists.txt"}
```
</details>

*./CMakeLists.txt* contains the solution build instructions:

<details><summary>CMakeLists.txt</summary>
```cmake
\include{"CMakeLists.txt"}
```
</details>


## Trademark notices

Registered trademarks and service marks are the property of their respective owners.
