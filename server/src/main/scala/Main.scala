import Protocol.NetMessage

object Main extends App {
  val msg = NetMessage.getDefaultInstance
  println("hi")
    
  val server = new ProtoBroker[NetMessage]
  server.run
}