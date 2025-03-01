// Jānis Lejnieks 4.grupa 211REC027
import scala.io.StdIn.readLine

object BinarySearchTree {
  
  def main(args: Array[String]): Unit = {
    println("Enter n (number count)")
    val n = readLine().toInt

    println(s"Enter $n numbers separated by spaces!")
    val numbers = readLine().split(" ").map(_.toInt).toList

    //Building BinaryTree
    val buildTree = numbers.foldLeft[Option[Node]](None)((tree, num) => Some(insert(tree, num)))

    println("Enter the number to search (x):")
    val x = readLine().toInt

    //Search for the value and print the result
    val result = searchPath(buildTree, x) //searches for result in searchPath
    println(s"result: $result")
  }

  case class Node(value: Int, left: Option[Node] = None, right: Option[Node] = None) // tree node
  //Option to avoid null(errors)

  //Inserting values in seatch tree
  def insert(node: Option[Node], value: Int): Node = {
    node match {
      case None => Node(value) //makes a new node if tree is empty
      case Some(n) =>   //if node exists goes to if condition    // Some indicates that value exists 
        if (value < n.value) //if the new value is less then nodes current value
          n.copy(left = Some(insert(n.left, value))) //Insert into left tree
        else
          n.copy(right = Some(insert(n.right, value))) //Insert into right tree
    }
  }

  //Searching for number in BinaryTree
  def searchPath(node: Option[Node], value: Int, path: String = "*"): String = {
    node match {
      case None => "not found" 
      case Some(n) => 
        if (value == n.value) path //Value found
        else if (value < n.value) searchPath(n.left, value, path + "L") //Search left
        else searchPath(n.right, value, path + "R") //Search right
    }
  }
}