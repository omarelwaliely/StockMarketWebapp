import AppRouter from "./pages/AppRouter";
import ThemeProvider from "./components/ThemeContext";

function App() {
  return (
    <div>
      <ThemeProvider>
      <AppRouter />
      </ThemeProvider>
    </div>
  );
}

export default App;
