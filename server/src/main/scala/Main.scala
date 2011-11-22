
class Bo {
  
}

object Main {
  def main(args: Array[String]) {
    println("hi")
    
    val server = new ProtoBroker[Bo]
    server.listen
  }
}