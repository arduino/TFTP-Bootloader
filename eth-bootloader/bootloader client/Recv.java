import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class Recv {

	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("Please specify receive port");
			System.out.println("Recv <port>");
			System.exit(1);
		}
		int port = Integer.parseInt(args[0]);
		recv(port);
	}

	public static void recv(int port) {
		try {
			byte[] buf = new byte[256];
			DatagramSocket socket = new DatagramSocket(port);
			DatagramPacket pack = new DatagramPacket(buf, buf.length);
			System.out.println("Listening on UDP port 5555.");
			System.out.println("Waiting for packets...");
			socket.receive(pack);

			// display response
			String received = new String(pack.getData(), 0, pack.getLength());
			System.out.println("Received: " + received);
			socket.close();
		} catch (SocketException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
