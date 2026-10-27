# USB Arsenal patches for Linux 4.14

Source: https://github.com/ravindu644/USB-Arsenal/tree/main/kernel-patches/4.14
Reference: https://github.com/ravindu644/USB-Arsenal/blob/main/kernel-patches/README.md

These are upstream git-format-patch files for the repository's Linux 4.14.357 source tree.
They are stored here for review and controlled application; **they have not been applied to
the kernel source by adding this directory alone**.

Apply from the kernel source root in numeric order, after checking applicability and current
source state:

```sh
git am kernel-patches/USB-Arsenal/4.14/*.patch
```

Included patch series:
- 0001/0002: configfs multiple-gadget creation and mainline gadget bring-up.
- 0003: Samsung NCM second-instance fix.
- 0004/0006: mass-storage CD-ROM and large DVD-ROM image support.
- 0007: configurable HID bInterval.
- 0008/0011: HID lifetime and rebind safety fixes.

Important: this repository's inspected default branch is Linux 4.14.357, not 4.19. These are
the 4.14 patch versions. Patch application and a kernel build must be verified before flashing.
