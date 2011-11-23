import Protocol.NetMessage
import akka.actor.Actor

class Player extends Actor {
  def receive = {
    case _ => println("recv msg")
  }
}

object Main extends App {
  val msg = NetMessage.getDefaultInstance
  println("hi")

    
  val server = new ProtoBroker[NetMessage]
  server.run[Player]
}