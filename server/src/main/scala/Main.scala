import Protocol.NetMessage
import akka.actor.Actor

class Player extends Actor {
  override def preStart() = {
    println(self + " connected")
  }

  override def postStop() = {
    println(self + " disconnected")
  }
  
  def receive = {
    case msg: NetMessage =>
      println(self + " received:")
      println(msg)
    
    case _ => println("recv unkown msg")
  }
}

object Main extends App {
  val server = new ProtoBroker
  server.run[Player](NetMessage.getDefaultInstance)
}
