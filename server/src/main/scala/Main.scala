import Protocol.NetMessage

class Bo {
  
}

object Main {
  def main(args: Array[String]) {
    val msg = NetMessage.getDefaultInstance
    println("hi")
    
    val server = new ProtoBroker[NetMessage]
    server.run
  }
}