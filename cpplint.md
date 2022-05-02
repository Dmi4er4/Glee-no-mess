```bash
$ pip install cpplint
```

```bash
$ cpplint --filter="-legal/copyright,-build/include_subdir" --exclude=./cmake-build-debug/ --exclude=./cmake-build-release/ --recursive ./
```