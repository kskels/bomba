import Protocol.NetMessage
import akka.actor.Actor
import java.net.InetSocketAddress

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
  val server = new ProtoBroker[NetMessage]
  server.run[Player](new InetSocketAddress(14242))
}
