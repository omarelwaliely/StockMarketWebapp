import { useEffect, useState } from 'react';

const useSocket = (url) => {
  const [socket, setSocket] = useState(null);

  useEffect(() => {
    const newSocket = new WebSocket(url);

    newSocket.addEventListener('open', () => {
      console.log('WebSocket connection opened');
    });

    newSocket.addEventListener('close', (event) => {
      console.log('WebSocket connection closed:', event);
    });

    newSocket.addEventListener('error', (event) => {
      console.error('WebSocket error:', event);
    });
    setSocket(newSocket);
    return () => {
      newSocket.close();
    };
  }, [url]);

  return socket;
};

export default useSocket;
