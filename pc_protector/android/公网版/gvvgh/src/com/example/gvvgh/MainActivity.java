package com.example.gvvgh;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {
	Button  mMsgSendBtn;
	Button  mMsgSendBtn2;
	Button  mMsgSendBtn3;
	Button  mMsgSendBtn4;
	Button  mMsgSendBtn5;
	Button  mMsgSendBtn6;
	Button  mMsgConnect;
	Button  mMsgClose;
	String mSendMsg="nibiru";
	String mSendMsg2="holmes";
	String mSendMsg3="kilmbr";
	String mSendMsg4="fixmbr";
	String mSendMsg5="photos";
	String mSendMsg6="shutdn";
	Socket g_socket;
	PrintWriter g_writer;
	private static int BUFFERSIZE=1024;  
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		final EditText editText=(EditText)findViewById(R.id.edit_text);  
		final EditText editPort=(EditText)findViewById(R.id.edit_port);  
		mMsgSendBtn = (Button)findViewById(R.id.send_msg);
		 mMsgSendBtn.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
					
					
				//	final String addr=editText.getText().toString();	
				//	final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //			Socket socket=new Socket(addr,Port);
							//			PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
										g_writer.println(mSendMsg);
										g_writer.flush();
									//	writer.close();
									//	socket.close();
					             }
					         });
					     newThread.start(); //启动线程
					
					
	}
	});
		 
	
		 mMsgSendBtn2 = (Button)findViewById(R.id.send_msg2);
		 mMsgSendBtn2.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
				//	final String addr=editText.getText().toString();
				//	final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //		Socket socket=new Socket(addr,Port);
							//		PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
									g_writer.println(mSendMsg2);
									g_writer.flush();
								//	writer.close();
								//	socket.close();
					             }
					         });
					     newThread.start(); //启动线程
	}});
		 mMsgSendBtn3 = (Button)findViewById(R.id.button1);
		 mMsgSendBtn3.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
				//	final String addr=editText.getText().toString();
				//	final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //	Socket socket=new Socket(addr,Port);
							//	PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
								g_writer.println(mSendMsg3);
								g_writer.flush();
							//	writer.close();
							//	socket.close();
					             }
					         });
					     newThread.start(); //启动线程
	}});
		 mMsgSendBtn4 = (Button)findViewById(R.id.button2);
		 mMsgSendBtn4.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
				//	final String addr=editText.getText().toString();
				//	final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //	Socket socket=new Socket(addr,Port);
							//	PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
								g_writer.println(mSendMsg4);
								g_writer.flush();
							//	writer.close();
							//	socket.close();
					             }
					         });
					     newThread.start(); //启动线程
				}});
		 
		 
	
		 
		 
		 mMsgSendBtn5 = (Button)findViewById(R.id.button3);
		 mMsgSendBtn5.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
					//final String addr=editText.getText().toString();
					//final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //这里写入子线程需要做的工作
					    	 try {
					    		 File sdCardDir = Environment.getExternalStorageDirectory().getAbsoluteFile();
									File file=new File(  
											sdCardDir+"/1.jpg");
									if (!file.exists()){
										
										file.createNewFile();
										
										
									}
									FileOutputStream out=new FileOutputStream(file);
								int readCount=0;
								byte[] fileData=new byte[4096];  

									//Socket socket=new Socket(addr,Port);
									g_socket.setReceiveBufferSize(100*1024);  
									InputStream dis= g_socket.getInputStream(); 
									//PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
									g_writer.println(mSendMsg5);
									g_writer.flush();
									
									
									
								
									/*readCount=dis.read(fileData,0,1024);
									out.write(fileData,0,readCount); 
									 out.flush();*/
								
											 
											 
											// TODO Auto-generated method stub
											while (true){
												
												//try {
													readCount=dis.read(fileData,0,4096);
													out.write(fileData,0,readCount); 
													 out.flush();
													 //if (readCount==0) break;
												//} catch (IOException e) {
													// TODO Auto-generated catch block
													//e.printStackTrace();
												//}  
												 /*由于Java的Socket是阻塞式的,所以最后需要发送确认信息让循环退出*/  
												if(new String(fileData, 0, readCount).equals("End!"))  {break; }
												 
												
											}
												
												
												 //fout.close(); 
												 //socket.close();
											//}
									    
											out.close(); 
											//socket.close();
									
											//writer.close();
									
									
								
									
									
									 
									  
									
									//writer.close();
									//socket.close();
									
									
								} catch (UnknownHostException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
					             }
					         });
					     newThread.start(); //启动线程
				}});
		 
		 
		 
		 
		 final EditText editCmd=(EditText)findViewById(R.id.edit_cmd); 
		 mMsgSendBtn5 = (Button)findViewById(R.id.button4);
		 mMsgSendBtn5.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
				//	final String addr=editText.getText().toString();
				//	final int Port=Integer.parseInt(editPort.getText().toString());
					final String Cmd=editCmd.getText().toString();
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //	Socket socket=new Socket(addr,Port);
							//	PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
								g_writer.println(mSendMsg6);
								g_writer.flush();
								g_writer.println(Cmd);
								g_writer.flush();
								//writer.close();
								//socket.close();
					             }
					         });
					     newThread.start(); //启动线程
				}});
		 
		 
		 mMsgConnect = (Button)findViewById(R.id.button5);
		 mMsgConnect.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
					final String addr=editText.getText().toString();
					final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //这里写入子线程需要做的工作
					    	 try {
									g_socket=new Socket(addr,Port);
								    g_writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(g_socket.getOutputStream())));
								  //  g_writer.println(mSendMsg2);
								//	g_writer.flush();
									
									//g_socket.close();
									
									
								} catch (UnknownHostException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								} catch (IOException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
					             }
					         });
					     newThread.start(); //启动线程
	}});
		 mMsgClose = (Button)findViewById(R.id.button6);
		 mMsgClose.setOnClickListener(new OnClickListener(){
				public void onClick(View v) {
				//	final String addr=editText.getText().toString();
				//	final int Port=Integer.parseInt(editPort.getText().toString());
					 Thread newThread; //声明一个子线程
					 newThread = new Thread(new Runnable() {
					     @Override
					             public void run() {
					             //		Socket socket=new Socket(addr,Port);
							//		PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())));
							//		g_writer.println(mSendMsg2);
							//		g_writer.flush();
									g_writer.close();
									try {
										g_socket.close();
									} catch (IOException e) {
										// TODO Auto-generated catch block
										e.printStackTrace();
									}
					             }
					         });
					     newThread.start(); //启动线程
	}});
		 
					     
	}}
	

		
	
	