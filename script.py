#!/usr/bin/env python3
import subprocess
import os
import sys
import argparse

def set_toolchain():
    """Set CMAKE_TOOLCHAIN_FILE if not set"""
    if 'CMAKE_TOOLCHAIN_FILE' not in os.environ:
        os.environ['CMAKE_TOOLCHAIN_FILE'] = '/home/caiof/vcpkg/scripts/buildsystems/vcpkg.cmake'

def run_command(cmd, cwd=None):
    """Run a shell command"""
    try:
        result = subprocess.run(cmd, shell=True, cwd=cwd, check=True, capture_output=True, text=True)
        print(result.stdout)
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(e.stderr)
        return False

def clean():
    """Clean build directory"""
    print("Cleaning build directory...")
    return run_command("rm -rf build")

def configure():
    """Configure the project"""
    print("Configuring project...")
    set_toolchain()
    return run_command("cmake -S . -B build")

def build():
    """Build the project"""
    print("Building project...")
    if not os.path.exists("build"):
        if not configure():
            return False
    return run_command("cmake --build build")

def run_app():
    """Run the application"""
    print("Running application...")
    return run_command("./build/convex_hull_3d")

def build_and_run():
    """Build and run"""
    if build():
        return run_app()
    return False

def main():
    parser = argparse.ArgumentParser(description="Build and run script for convex_hull_3d")
    parser.add_argument('action', choices=['build', 'clean', 'run', 'build-run'], help='Action to perform')

    args = parser.parse_args()

    if args.action == 'clean':
        success = clean()
    elif args.action == 'build':
        success = build()
    elif args.action == 'run':
        success = run_app()
    elif args.action == 'build-run':
        success = build_and_run()
    else:
        print("Invalid action")
        success = False

    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()