#!/usr/bin/env python3
from os.path import exists as _exists
from re import compile as _re_compile, DOTALL as _re_DOTALL
from ast import literal_eval
from zlib import compress

# Either src/main.c or src/main.cpp
SOURCE = "src/main.c"
if not _exists(SOURCE) and _exists(SOURCE+"pp"):
	SOURCE += "pp"

with open("src/main.c") as f:
	source_code = f.read()

def search(pattern, error_msg, what=lambda m: m.group(1)):
	m = pattern.search(source_code)
	assert m, error_msg
	return what(m)

LITERAL_EVAL = lambda m: literal_eval(m.group(1))

# find tic_script definition
definition = search(_re_compile(r"TIC_EXPORT const tic_script EXPORT_SCRIPT\([^)]+\) = \{(.+)\};", flags=_re_DOTALL), "Couldn't find definition!")

# Language identifier
lang = search(_re_compile(r"\.id = (0x[0-9A-Fa-f]+|\d+),?"), "Couldn't find language code!", LITERAL_EVAL)
assert lang<256, "Language code must be 1 byte in length!"

# Language name
name = search(_re_compile(r"\.name = (\"([^\"]+)\"),?"), "Couldn't find language name!", LITERAL_EVAL)

# Single comment
singleComment = search(_re_compile(r"\.singleComment = (\"([^\"]+)\"),?"), "Couldn't find singleComment!", LITERAL_EVAL)

# Now to construct the TIC-80 cart:
# - CHUNK_DEFAULT (`11 00 00 00`) to use the new default stuff
cart = bytearray([17, 0, 0, 0])
# - CHUNK_LANG (`14 01 00 00 <lang>`) to say it's the language
cart.extend([20, 1, 0, 0, lang])
# - CHUNK_CODE (`05 xx xx 00 <code>`) because otherwise TIC-80 complains the code is empty
#   (code is `<singleComment> script: <name>`)
code = f"{singleComment} script: {name}\n".encode("ascii")
cart.extend([5])
cart.extend(divmod(len(code),256)[::-1]) # little endian
cart.extend([0])
cart.extend(code)

compressed = compress(cart, level=9)

dat = ""
for byte in compressed:
	dat += f"0x{byte:02x}, "

with open("src/democart.dat", "w") as f:
	f.write(dat)
