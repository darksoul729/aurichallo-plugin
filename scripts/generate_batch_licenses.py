#!/usr/bin/env python3
"""
Auric Halo - Batch License Key Generator
Generate multiple license keys from a list of Hardware IDs
"""

import sys
from generate_license_key import generate_license_key, save_to_file

def main():
    print("=" * 50)
    print("  AURIC HALO - BATCH LICENSE GENERATOR")
    print("=" * 50)
    print()
    
    if len(sys.argv) < 2:
        print("Usage: python3 generate_batch_licenses.py customers.txt")
        print()
        print("File format (one Hardware ID per line):")
        print("  A1B2C3D4E5F6G7H8")
        print("  B2C3D4E5F6G7H8I9")
        print("  C3D4E5F6G7H8I9J0")
        print()
        sys.exit(1)
    
    filename = sys.argv[1]
    
    try:
        with open(filename, 'r') as f:
            hardware_ids = [line.strip() for line in f if line.strip()]
    except FileNotFoundError:
        print(f"ERROR: File '{filename}' not found")
        sys.exit(1)
    
    print(f"Found {len(hardware_ids)} Hardware IDs")
    print()
    
    generated = []
    for i, hwid in enumerate(hardware_ids, 1):
        if len(hwid) < 8:
            print(f"⚠️  Skipping invalid Hardware ID: {hwid}")
            continue
        
        license_key = generate_license_key(hwid)
        save_to_file(hwid, license_key)
        generated.append((hwid, license_key))
        print(f"{i}. {hwid} → {license_key}")
    
    print()
    print("=" * 50)
    print(f"✅ Generated {len(generated)} license keys")
    print(f"📝 All keys saved to: license_keys.txt")
    print()

if __name__ == "__main__":
    main()
