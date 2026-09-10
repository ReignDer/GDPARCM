# Requirement
Requires any version of MSVC, recommended version is 2026, or the xmake will not build

# How to Set-Up
1. Go to Scripts Folder
3. Click the setup-windows bat file -- this is a bootstrap for xmake
4. Allow changes for the xmake
5. You will open a wizard set-up, Click on next
6. Click I Agree
7. Check Add to Path
8. Set a folder you want to install xmake
9. Install xmake
10. Click Finish
11. IDE project files for MSVC will be automatically generated
12. There will be a folder with the prefix "vsxmake" following with the current MSVC version e.g. vsxmake2026, vsxmake2022. Select that folder and the .sln file is found there

Only Supports Windows

If it cannot find the submodules use the command `git submodule update --init --recursive` to clone necessary modules
