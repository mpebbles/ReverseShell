import java.net.*;
import java.io.*;
import java.util.Scanner;
 
public class Receiver {
    public static void main(String[] args) throws IOException {
        
        boolean loop = true;
         
        if (args.length != 1) {
            System.err.println("Usage: java Receive <port number>");
            System.exit(1);
        }
        
        System.out.println("Waiting...");
         
        int portNumber = Integer.parseInt(args[0]);
        Scanner stdIn = new Scanner(System.in);
        
        try (
            ServerSocket socket =
                new ServerSocket(Integer.parseInt(args[0]));
            Socket clientSocket = socket.accept();
            PrintWriter out =
                new PrintWriter(clientSocket.getOutputStream(), true);                   
            BufferedReader in = new BufferedReader(
                new InputStreamReader(clientSocket.getInputStream()));
        ) {
            String inputLine;

            while ((inputLine = in.readLine()) != null) {
                if(inputLine.equals(";;;")) {
                    System.out.print("System > ");
                    out.println(stdIn.nextLine());
                } else {
                    // Space added to make it easy to read
                    System.out.println("  " + inputLine);
                }
            }

        } catch (IOException e) {System.out.println("An error occurred");}
    }
}
