#!/usr/bin/env python3

import argparse
import subprocess
from pathlib import Path

WARMUP = 5_000_000
SIM = 20_000_000

"./bin/champsim -w $WARMUP -i $SIM $1 --json 1.json"


def main():
    parser = argparse.ArgumentParser(
        description="Tool to run champsim on tracelist"
    )

    parser.add_argument(
        "-b", "--binary", type=Path, required=True, help="Champsim binary"
    )

    parser.add_argument(
        "-t",
        "--traces-dir",
        type=Path,
        required=True,
        help="Directory with traces",
    )

    parser.add_argument(
        "-o",
        "--out-dir",
        type=Path,
        required=True,
        help="Directory to put output json files",
    )

    args = parser.parse_args()

    if not args.binary.is_file():
        raise ValueError(f"File does not exist: {args.binary}")

    champsim: Path = args.binary
    traces_dir: Path = args.traces_dir
    out_dir: Path = args.out_dir

    if not traces_dir.is_dir():
        raise ValueError(f"Not a directory: {traces_dir}")

    out_dir.mkdir(parents=True, exist_ok=True)

    for trace in traces_dir.iterdir():
        if trace.suffix == ".xz":
            print(f"Running trace {trace}")
            cmd = [
                champsim.as_posix(),
                "-w",
                str(WARMUP),
                "-i",
                str(SIM),
                trace.as_posix(),
                "--json",
                (out_dir / f"{trace.stem}.json").as_posix(),
            ]
            print(f"Command: {' '.join(cmd)}")
            subprocess.run(
                cmd,
                check=True,
            )

    print(f"Done. Result json written to {out_dir}")


if __name__ == "__main__":
    main()
