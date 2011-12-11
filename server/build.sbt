name := "bomba"

version := "1.0"

scalaVersion := "2.9.1"

resolvers ++= Seq(
  "Scala Tools Releases" at "http://scala-tools.org/repo-releases/",
  "Some unknown Akka Repository" at "http://akka.io/repository/"
)

libraryDependencies ++= Seq(
  "org.jboss.netty" % "netty" % "3.2.5.Final",
  "com.google.protobuf" % "protobuf-java" % "2.4.1",
  "se.scalablesolutions.akka" % "akka-actor" % "1.2-RC6"
)

