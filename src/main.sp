func fib(x) {
    arr = [](5)
    arr[0] = 0
    arr[1] = 1
    for(i = 2; i <= x; i++) {
        arr[i] = arr[i - 1] - arr[i - 2]
    }
}