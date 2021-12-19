# Interval-Tree
C++
## Objective

![Screenshot from 2021-12-10 16-07-13](https://user-images.githubusercontent.com/66109376/145539472-be02cca1-4872-4e00-b91f-a5089c40a4cd.png)

## Input

![Screenshot from 2021-12-10 16-09-16](https://user-images.githubusercontent.com/66109376/145539679-a07ead6e-809e-4057-8ee4-f4a1201eb444.png)

### Explanation of the Input
* Each line defines the action mode, rectangle ID, coordinate of lower-left corner and coordinate of upper-right corners.
* action mode
  * I: Insert the rectangle
  * D: Delete the rectangle
  * O: Overlap detection
  * S: Search the rectangle
  * A: Area. Find the rectangle which area is smallest with the same given coordinate of lower-left corner rectangle

## Output

![image](https://user-images.githubusercontent.com/66109376/146669410-66ff0d05-7744-4db5-8664-0934b56a3431.png)

### Explanation of the Output
* Only output when the action mode is O, S and A.
* In each action, the first line output the action mode.
* If the mode is O, please output each rectangle ID that the rectangle overlaps with
the pattern rectangle and output “violate” if one of the edge is smaller than the
distance constraint. Only output the rectangle which cover the pattern rectangle.
Please output the rectangle ID in ascending order.

**Distance constraint is set as 20.**
