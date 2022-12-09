PYTHON=./.venv/bin/python
.PHONY: run
run: ./src/main.py
	$(PYTHON) ./src/main.py 
clean:
	rm -rf .venv
setup:
	$(PTYHON) -m pip install -r requirements.txt
init:
	virtualenv .venv
show:
	eog .github/euclidean.png
