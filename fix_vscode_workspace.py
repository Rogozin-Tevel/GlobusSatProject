import os
import json
import subprocess

if not os.path.exists(".vscode"):
    os.makedirs(".vscode")

data = {
    "C_Cpp.default.includePath": [
        "GlobusSatProject/src",
        "hal/at91/include",
        "hal/freertos/include",
        "hal/hal/include",
        "hal/hcc/include",
        "satellite-subsystems/satellite-subsystems/include"
    ],
    "files.exclude": {
        ".vscode": True,
        ".git": True,
        ".gitignore": True,
        "*.py": True,
        "*.md": True
    },
    "editor.minimap.enabled": False
}

with open(".vscode/settings.json", "w") as write_file:
    json.dump(data, write_file, indent=4)