import Protocol.NetMessage
import akka.actor.Actor

class Player extends Actor {
  override def preStart() = {
    println("connected")
  }

  override def postStop() = {
    println("disconnected")
  }
  
  def receive = {
    case msg: NetMessage => println("got netmessage " + msg.getType())
    case _ => println("recv unkown msg")
  }
}

object Main extends App {
  val msg = NetMessage.getDefaultInstance
  val server = new ProtoBroker
  server.run[Player](NetMessage.getDefaultInstance)
}
