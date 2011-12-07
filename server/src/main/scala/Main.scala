import Protocol.NetMessage
import akka.actor.Actor

class Player extends Actor {
  def receive = {
    case Connected() => println("connected!")
    case Some(msg: NetMessage) => println("got netmessage")
    case _ => println("recv unkown msg")
  }
}

object Main extends App {
  val msg = NetMessage.getDefaultInstance
  val server = new ProtoBroker
  server.run[Player](NetMessage.getDefaultInstance)
}
