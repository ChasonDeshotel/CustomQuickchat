#pragma once

void
CheckJsonFiles();
void
GetFilePaths();
void
ReadDataFromJson();

void
UpdateBindingsData();
void
WriteBindingsToJson();
void
WriteVariationsToJson();

fs::path customQuickchatFolder;
fs::path bindingsFilePath;
fs::path variationsFilePath;
