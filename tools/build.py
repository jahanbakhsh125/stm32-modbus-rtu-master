#!/usr/bin/env python3
"""Build the bundled STM32 project with a locally installed Arm GCC toolchain."""
from pathlib import Path
import argparse
import os
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]
NAME = 'modbus_rtu_7'
INCLUDES = ['Core/Inc', 'modbus', 'Drivers/STM32F1xx_HAL_Driver/Inc', 'Drivers/STM32F1xx_HAL_Driver/Inc/Legacy', 'Drivers/CMSIS/Device/ST/STM32F1xx/Include', 'Drivers/CMSIS/Include']
HAL = True

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--profile", choices=["Debug", "Release"], default="Debug")
    args = parser.parse_args()
    gcc = os.environ.get("ARM_GCC") or shutil.which("arm-none-eabi-gcc")
    if not gcc:
        parser.error("Install Arm GCC or set ARM_GCC to the full compiler executable path.")
    base = ROOT / "firmware" / NAME
    build = ROOT / "build" / args.profile
    build.mkdir(parents=True, exist_ok=True)
    temp = build / "tmp"
    temp.mkdir(exist_ok=True)
    env = os.environ.copy()
    env.update(TMP=str(temp), TEMP=str(temp), TMPDIR=str(temp))
    flags = ["-mcpu=cortex-m3", "-mthumb", "-std=gnu11",
             "-Og" if args.profile == "Debug" else "-Os", "-g0",
             "-ffunction-sections", "-fdata-sections", "-Wall", "-Wextra",
             "-DSTM32F103xB"]
    if HAL:
        flags.append("-DUSE_HAL_DRIVER")
    if args.profile == "Debug":
        flags.append("-DDEBUG")
    flags.extend("-I" + str(base / p) for p in INCLUDES)
    sources = sorted(p for p in base.rglob("*") if p.suffix in (".c", ".s"))
    objects = []
    subprocess.run([gcc, "--version"], check=True, env=env)
    for i, source in enumerate(sources):
        obj = build / (str(i) + ".o")
        subprocess.run([gcc, *flags, "-c", str(source), "-o", str(obj)],
                       check=True, env=env, timeout=120)
        objects.append(str(obj))
    subprocess.run([gcc, "-mcpu=cortex-m3", "-mthumb", *objects,
                    "-T" + str(base / "STM32F103C8TX_FLASH.ld"),
                    "--specs=nano.specs", "--specs=nosys.specs",
                    "-Wl,--gc-sections", "-static", "-Wl,--start-group",
                    "-lc", "-lm", "-Wl,--end-group",
                    "-o", str(build / "firmware.elf")],
                   check=True, env=env, timeout=120)
    print("Built:", build / "firmware.elf")

if __name__ == "__main__":
    main()
