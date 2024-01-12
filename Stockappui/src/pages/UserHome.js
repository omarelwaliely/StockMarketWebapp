import React, { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';

const UserHome = () => {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const navigate = useNavigate();
  useEffect(() => {
    setIsLoggedIn(true);
  }, []);
  useEffect(() => {
    if (!isLoggedIn) {
      navigate('/login');
    }
  }, [isLoggedIn, navigate]);

  return (
    <div>
      {isLoggedIn ? (
        <div> Welcome to User Home! </div>
      ) : (
        <div> Redirecting to login... </div>
      )}
    </div>
  );
};

export default UserHome;
