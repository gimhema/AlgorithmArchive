
## Chroma Key to Alpha

### Install
```
pip install pillow
```

### Usage
```
python chroma_to_to_alpha.py input.jpg output.png
python chroma_to_alpha.py input.jpg output.png --key green --threshold 95 --feather 10 --despill 0.35
python chroma_to_alpha.py input.jpg output.png --key 0,0,255
python chroma_to_alpha.py ./frames ./out --key auto --threshold 90 --feather 12
```

