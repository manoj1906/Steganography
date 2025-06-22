# Steganography
A project based on practice of concealing a file, message, image within another file, message, image.
Project based on Encrypting secret message in a BMP image file without effecting the image view. Uses : Adding a watermark -- a trademark or other identifying data hidden in multimedia or other content files -- is one common use of steganography. Watermarking is a technique often used by online publishers to identify the source of media files that have been found being shared without permission. Programming Language : C Programming Language. Operation:

1]Encode: Encoding Secret message from secret.txt in the beautiful.bmp file.
2]Decode: Decoding the secret message from encoded image file to a new(.txt) file.

# 🔐 Steganography in BMP Images using C

Hide and retrieve secret messages within BMP image files without altering their visual appearance.

---

## 📌 Overview

This C-based project demonstrates **image steganography**, allowing users to **embed secret messages inside BMP image files** (`.bmp`) and later **decode** them. The image retains its original look, making the hidden message undetectable to the human eye.

---

## 🚀 Features

- 🔏 **Encoding Mode**: Hide the contents of `secret.txt` inside a 24-bit BMP image (`beautiful.bmp`).
- 🔓 **Decoding Mode**: Extract the hidden message from the encoded image and save it to a text file.
- 🖼 **Visual Integrity**: The image remains visually unchanged after encoding.
- 💧 **Watermarking Application**: Useful for embedding invisible digital watermarks to claim content ownership or track media usage.

---

## 🛠 Requirements

- GCC or any C compiler
- A 24-bit BMP image (e.g., `beautiful.bmp`)
- A text file containing the secret message (e.g., `secret.txt`)

---

## ⚙️ Compilation

Use `gcc` to compile the project:

gcc steganography.c -o steganography
🧪 Usage
▶️ Encode (Hide Secret Message)

./steganography encode beautiful.bmp secret.txt encoded.bmp
beautiful.bmp: Original cover image

secret.txt: Text file with your secret message

encoded.bmp: Output BMP file with hidden message

◀️ Decode (Extract Secret Message)

./steganography decode encoded.bmp output.txt
encoded.bmp: Image containing the hidden message

output.txt: Output file where the extracted message will be saved

🧠 How It Works
The program modifies the least significant bits (LSBs) of the BMP pixel data to embed each bit of the secret message. This allows data to be hidden without noticeably altering the image.

📌 Example
You have:

beautiful.bmp (cover image)

secret.txt (contains "The key is under the bridge.")

Run encode:

./steganography encode beautiful.bmp secret.txt encoded.bmp
Send encoded.bmp to someone.
They run:

./steganography decode encoded.bmp output.txt
output.txt now contains the secret message.


🙌 Acknowledgments
Inspired by the art of steganography and the need for secure communication without raising suspicion.
---
