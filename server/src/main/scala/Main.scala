import akka.actor.Actor
import Protocol.NetMessage

class Player extends Actor {
  import Protocol.PlayerInfo

  val print = (msg: String) => println(self + " " + msg)
  val makePlayerInfo = (localId: Int) =>
    NetMessage.newBuilder
      .setType(NetMessage.Type.PLAYER_INFO)
      .setPlayerInfo(PlayerInfo.newBuilder
        .setLocalId(localId))
      .build
    
  override def preStart() = {
    print("initialize")
  }

  override def postStop() = {
    print("shutdown")
  }
  
  def receive = {
    case (remote: Endpoint[NetMessage], msg) =>
      print("received msg from " + remote + ":")
      print(msg.toString)
      remote.send(makePlayerInfo(1337))
    
    case Connected(remote) =>
      print("connected to " + remote)
    
    case _ => println("recv unkown msg")
  }
}

object Main extends App {
  import java.net.InetSocketAddress

  val server = new ProtoBroker[NetMessage]
  server.run[Player](new InetSocketAddress(14242))
}
