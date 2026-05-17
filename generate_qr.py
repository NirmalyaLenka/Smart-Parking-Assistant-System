"""
generate_qr.py — QR Code Generator for Smart Parking Assistant

This script generates a QR code image pointing to your deployed parking frontend.
When drivers scan this code at the entrance, their phone opens the live slot map.

Requirements:
    pip install qrcode[pil]

Usage:
    1. Set PARKING_URL to your server's address (local IP or public domain)
    2. Run: python generate_qr.py
    3. Two PNG files will be created: one for two-wheelers, one for four-wheelers

The generated images are 1000x1000 pixels, suitable for A4 printing.
Get them laminated before placing at the entrance.
"""

import qrcode
from PIL import Image, ImageDraw, ImageFont
import os

# Update this to your actual server address
# For local network: "http://192.168.1.100:3000"
# For internet-facing deployment: "https://yourdomain.com/parking"
PARKING_URL = "http://192.168.1.100:3000"

# Output directory (same folder as this script)
OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))


def generate_qr(url, label, filename):
    """Generate a QR code with a label below it."""

    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_H,
        box_size=10,
        border=4,
    )
    qr.add_data(url)
    qr.make(fit=True)

    qr_img = qr.make_image(fill_color="black", back_color="white").convert("RGB")

    # Create a canvas with room for label below
    canvas_width  = 1000
    canvas_height = 1100
    canvas = Image.new("RGB", (canvas_width, canvas_height), "white")

    # Center the QR code on the canvas
    qr_resized = qr_img.resize((900, 900))
    x_offset = (canvas_width - 900) // 2
    canvas.paste(qr_resized, (x_offset, 40))

    # Add label text
    draw = ImageDraw.Draw(canvas)
    try:
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 48)
    except IOError:
        font = ImageFont.load_default()

    text_bbox = draw.textbbox((0, 0), label, font=font)
    text_width = text_bbox[2] - text_bbox[0]
    text_x = (canvas_width - text_width) // 2
    draw.text((text_x, 960), label, fill="black", font=font)

    output_path = os.path.join(OUTPUT_DIR, filename)
    canvas.save(output_path, "PNG", dpi=(300, 300))
    print(f"Saved: {output_path}")


if __name__ == "__main__":
    print("Generating QR codes...")
    print(f"URL: {PARKING_URL}")

    generate_qr(
        url=PARKING_URL + "?type=4W",
        label="Scan for Four-Wheeler Parking",
        filename="qr_four_wheeler.png"
    )

    generate_qr(
        url=PARKING_URL + "?type=2W",
        label="Scan for Two-Wheeler Parking",
        filename="qr_two_wheeler.png"
    )

    print("\nDone. Print and laminate these files for the entrance gate.")
    print("Recommended print size: A4 or A5")
