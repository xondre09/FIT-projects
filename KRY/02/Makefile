PYTHON=python3
MAKE=make
PROGRAM=decipher.py
LOGIN=xondre09

default:

decipher:
	@$(PYTHON) $(PROGRAM) "$(publicKey)"

test:
	$(MAKE) -s decipher publicKey="(0x47776904c0f1cc3a9c0984b66f75301a5fa68678f0d64af8ba1abce34738a73e, 0xaa005ee6b5b957286231856577648e8381b2804428d5733f32f787ff71f1fcdc)"
	$(MAKE) -s decipher publicKey="(0x52910a011565810be90d03a299cb55851bab33236b7459b21db82b9f5c1874fe, 0xe3d03339f660528d511c2b1865bcdfd105490ffc4c597233dd2b2504ca42a562)"
pack:
	tar -cvzf $(LOGIN).tgz Makefile $(PROGRAM)

clean:
	rm -f $(LOGIN).tgz

.PHONY: decipher test pack
