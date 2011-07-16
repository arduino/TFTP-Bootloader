import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class Send {

	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("Please specify send address and port");
			System.out.println("Send <addr> <port>");
			System.exit(1);
		}
		try {
			InetAddress address = InetAddress.getByName(args[0]);
			int port = Integer.parseInt(args[1]);
			send(address, port);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
	}

	public static void send(InetAddress _addr, int _port) {

		try {
			byte[] magic = { 'a', 'r', 'd', 'u', 'i', 'n', 'o' };
			DatagramPacket packet;
			packet = new DatagramPacket(magic, magic.length, _addr, _port);

			DatagramSocket socket = new DatagramSocket();
			socket.send(packet);
			socket.close();
			System.out.println("Packet sent.");
		} catch (SocketException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
