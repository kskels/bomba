import akka.actor.Actor

/**
 * Starts a server and receives protobuf M type messages.
 * Don't rely on TCP features in case we need to migrate to UDP.
 * The server receives messages and forwards them to the session's actor. If
 * the actor dies, any session metadata is removed.
 * Note that the server might be multithreaded.
 */
class ProtoBroker[M] {
  def run[T <: Actor](implicit m: Manifest[T]) {
    // startup server, wait for connections
    
    val actor = Actor.actorOf[T].start
    actor ! "welcome!"
    
    println("*** server starting")
  }
}