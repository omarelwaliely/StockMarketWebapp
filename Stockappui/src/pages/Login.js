import React, { useState } from 'react';
import Avatar from '@mui/material/Avatar';
import Button from '@mui/material/Button';
import CssBaseline from '@mui/material/CssBaseline';
import TextField from '@mui/material/TextField';
import { Link, useNavigate } from 'react-router-dom';
import Box from '@mui/material/Box';
import Typography from '@mui/material/Typography';
import Container from '@mui/material/Container';
import useSocket from '../services/useSocket';

const Login = () => {
  const navigate = useNavigate();
  const socket = useSocket('ws://localhost:3001');
  const [formData, setFormData] = useState({
    firstName: '',
    lastName: '',
    email: '',
    password: '',
  });

  const handleChange = (event) => {
    const { name, value } = event.target;
    setFormData((prevData) => ({ ...prevData, [name]: value }));
  };

  const handleSubmit = async (event) => {
    event.preventDefault();

    try {
      if (socket.readyState === WebSocket.OPEN) {
        const message = {
          endpoint: '/login',
          header: 'token',
          body: formData,
        };
        socket.send(JSON.stringify(message));
        await new Promise((resolve) => {
          socket.addEventListener('message', (event) => {
            const data = JSON.parse(event.data);
            if (data.code === 200) {
              console.log('Login successful!');
              navigate("/managestock")
              console.log(data.body.token);
            } else {
              console.error('Login failed:', data.body.message);
            }
            resolve();
          });
        });
      } else {
        console.error('WebSocket connection is not open');
      }
    } catch (error) {
      console.error('Error during Login:', error);
    }
  };

  return (
    <Container component="main" maxWidth="xs">
      <CssBaseline />
      <Box
        sx={{
          marginTop: 8,
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          backgroundColor: '#333',
          color: '#fff', 
          padding: '20px',
          borderRadius: '10px',
          boxShadow: '0px 0px 10px rgba(0, 0, 0, 0.1)',
        }}
      >
        <Avatar sx={{ m: 1, bgcolor: 'primary.main' }}></Avatar>
        <Typography component="h1" variant="h5">
          Log in
        </Typography>
        <form onSubmit={handleSubmit} noValidate>
          <TextField
            required
            fullWidth
            id="email"
            label="Email Address"
            name="email"
            autoComplete="email"
            value={formData.email}
            onChange={handleChange}
          />
          <TextField
            required
            fullWidth
            name="password"
            label="Password"
            type="password"
            id="password"
            autoComplete="new-password"
            value={formData.password}
            onChange={handleChange}
          />
          <Button
            type="submit"
            fullWidth
            variant="contained"
            sx={{ mt: 3, mb: 2 }}
          >
            Log In
          </Button>
        </form>
        <Link to="/register" variant="body2" style={{ color: '#fff' }}>
          Don't have an account? Sign up
        </Link>
      </Box>
    </Container>
  );
};

export default Login;
