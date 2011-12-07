import akka.actor.{Actor, ActorRef}

import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory
import org.jboss.netty.bootstrap.ServerBootstrap
import org.jboss.netty.channel.{
  ChannelPipelineFactory,
  Channels
}

import org.jboss.netty.handler.codec.protobuf.{
  ProtobufDecoder,
  ProtobufEncoder,
  ProtobufVarint32FrameDecoder,
  ProtobufVarint32LengthFieldPrepender
}

import java.net.InetSocketAddress
import java.util.concurrent.Executors
import com.google.protobuf.MessageLite

/**
 * Starts a server and receives protobuf messages.
 * Don't rely on TCP features in case we need to migrate to UDP.
 * The server receives messages and forwards them to the session's actor. If
 * the actor dies, any session metadata is removed.
 * Note that the server might be multithreaded.
 */
class ProtoBroker {
  def run[T <: Actor](defaultInstance: MessageLite)(implicit m: Manifest[T]) {
    val bootstrap = new ServerBootstrap(new NioServerSocketChannelFactory(
      Executors.newCachedThreadPool(), Executors.newCachedThreadPool()))

    bootstrap.setPipelineFactory(new ChannelPipelineFactory {
      def getPipeline = {
        val p = Channels.pipeline()
        p.addLast("frameDecoder", new ProtobufVarint32FrameDecoder())
        p.addLast("protobufDecoder", new ProtobufDecoder(defaultInstance))
        p.addLast("frameEncoder", new ProtobufVarint32LengthFieldPrepender())
        p.addLast("protobufEncoder", new ProtobufEncoder)
        p.addLast("handler", new ChannelActorGlue(Actor.actorOf[T].start))
        p
      }
    })

    bootstrap.bind(new InetSocketAddress(14242))
    
    println("*** server starting")
  }
}

import org.jboss.netty.channel.{
  SimpleChannelUpstreamHandler,
  ChannelHandlerContext,
  ChannelStateEvent,
  MessageEvent
}

case class Connected()
case class ReceivedMsg(msg: Object)  // TODO: make this one only receive the proper protobuf type

class ChannelActorGlue(actor: ActorRef) extends SimpleChannelUpstreamHandler {
  override def channelConnected(ctx: ChannelHandlerContext, e: ChannelStateEvent) {
    actor ! Connected()
  }

  override def channelDisconnected(ctx: ChannelHandlerContext, e: ChannelStateEvent) {
    actor.stop()
  }

  override def messageReceived(ctx: ChannelHandlerContext, e: MessageEvent) {
    actor ! ReceivedMsg(e.getMessage)
  }
}
