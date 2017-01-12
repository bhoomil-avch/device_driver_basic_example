# impp-edge
EDGE device software for IMPP

- Tested with Python 3.5.2 version.
- Dependencies for impp-edge for both Linux and Windows host machine.
  Python version specific dependencies for opcua lib from python-opcua,
  For Python > 3.4
  - cryptography
  - python-dateutil
  - pytz
  For Python 2.7 or < 3
  - enum34
  - trollius(asyncio)
  - futures(concurrent.futures)
  dependencies for IoT Data Distribution module,
  - requests
  All above modules can be install using "pip install (ModuleName)"
  One more dependency is web.py module, install that using following syntax,
  - easy_install web.py