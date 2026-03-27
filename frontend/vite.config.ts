import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'

// https://vite.dev/config/
export default defineConfig({
  plugins: [svelte()],
  build: {
    // Output directly to the PlatformIO data folder
    outDir: '../data', 
    // Wipe the old UI files before building the new ones
    emptyOutDir: true, 
  }
})
