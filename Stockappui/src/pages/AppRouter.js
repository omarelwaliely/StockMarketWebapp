import React from 'react';
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import Register from './Register';
import Login from './Login';
import UserHome from './UserHome';
import StockPage from './ManageStocks';

const AppRouter = () => {

  return (
    <BrowserRouter>
      <Routes>
      <Route path="/register" element= {<Register />} />
      <Route path="/login" element={<Login />} index={true} />
      <Route path = "/" element = {<UserHome />} />
      <Route path = "/managestock" element = {<StockPage />} />
      </Routes>
    </BrowserRouter>
  );
};

export default AppRouter;
