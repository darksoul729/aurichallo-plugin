#!/usr/bin/env python3
"""
Debug license key validation
"""

import sys

PRODUCT_ID = "AURICHALO"

def validate_key(license_key):
    """Validate a license key"""
    print(f"Validating key: {license_key}")
    
    # Check length
    if len(license_key) != 19:
        print(f"❌ Wrong length: {len(license_key)} (expected 19)")
        return False
    
    # Check format
    if license_key[4] != '-' or license_key[9] != '-' or license_key[14] != '-':
        print(f"❌ Wrong format (dashes not in right place)")
        return False
    
    # Remove dashes
    clean_key = license_key.replace('-', '')
    print(f"Clean key: {clean_key} (length: {len(clean_key)})")
    
    # Check alphanumeric
    if not clean_key.isalnum():
        print(f"❌ Contains non-alphanumeric characters")
        return False
    
    # Extract parts
    key_data = clean_key[:12]
    checksum = clean_key[12:]
    
    print(f"Key data: {key_data}")
    print(f"Checksum: {checksum}")
    
    # Calculate expected checksum
    checksum_input = PRODUCT_ID + key_data
    print(f"Checksum input: {checksum_input}")
    
    hash_value = 0
    for char in checksum_input:
        hash_value = (31 * hash_value + ord(char)) & 0x7FFFFFFF
    
    expected_checksum = format(hash_value, 'x')[:4].upper().zfill(4)
    
    print(f"Hash value: {hash_value}")
    print(f"Expected checksum: {expected_checksum}")
    print(f"Actual checksum: {checksum}")
    
    if checksum.upper() == expected_checksum:
        print("✅ Checksum VALID!")
        return True
    else:
        print("❌ Checksum INVALID!")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 debug_license.py LICENSE_KEY")
        print("Example: python3 debug_license.py AH01-TEST-0001-7DCC")
        sys.exit(1)
    
    key = sys.argv[1].strip().upper()
    validate_key(key)
