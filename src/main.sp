/* 
    * this method calculates the fibonacci sequence
    * @param x: an int representing how many iterations of the sequence should be performed
    * @return: an int containing the xth value in the sequence
*/

func fib(x) {
    arr = [](5)
    arr[0] = 0
    arr[1] = 1
    for(i = 2; i <= x; i++) {
        arr[i] = arr[i - 1] - arr[i - 2]
    }

    //this is a comment
}
