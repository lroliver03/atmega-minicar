# Python libraries

A library is a collection of Python modules, and a Python module is a `.py` file that contains code.

## How to create a library

To add a library, create a folder as follows.

```python
libraries\
|-- [library name]\
|   |-- __init__.py # Is executed when you import the library.
|   |-- *.py # Individual modules.
```

In `__init__.py`, you should import the contents of the library.

### Example
> ```python
> libraries\
> |-- examplelib\
> |   |-- __init__.py
> |   |-- examplemod1.py
> |   |-- examplemod2.py
> ```
> 
> Contents of `__init__.py`:
> ```python
> import examplemod1 # Import the entirety of examplemod1
> from examplemod2 import ... # Import specific contents from examplemod2
> ```
> 
> In your script:
> ```python
> import libraries.examplelib # Will execute __init__.py
> ```
>
> The library `examplelib`, which contains modules `examplemod1` and `examplemod2`, has been imported.