import akka.actor.Actor

/**
 * Starts a server and receives protobuf M type messages.
 * Don't rely on TCP features in case we need to migrate to UDP.
 */
class ProtoBroker[M] {
  def run[T <: Actor](implicit m: Manifest[T]) {
    val actor = Actor.actorOf[T].start
    actor ! "welcome!"
    
    println("*** server starting")
  }
}