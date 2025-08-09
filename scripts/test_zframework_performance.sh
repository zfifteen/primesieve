#!/bin/bash
# Z Framework Quick Performance Test
# Runs key benchmarks to demonstrate performance characteristics

echo "🧪 Z Framework Quick Performance Validation"
echo "============================================"
echo ""

# Compile the comprehensive benchmark if needed
if [ ! -f comprehensive_benchmark ]; then
    echo "📦 Compiling benchmark suite..."
    gcc -O3 -I./include -o comprehensive_benchmark comprehensive_benchmark.c libprimesieve.a -lm -lstdc++
    if [ $? -ne 0 ]; then
        echo "❌ Failed to compile benchmark"
        exit 1
    fi
fi

echo "🚀 Running quick performance test..."
echo ""

# Run a subset of key tests
./comprehensive_benchmark | head -50

echo ""
echo "✅ Quick test completed!"
echo ""
echo "📋 For full comprehensive analysis, run:"
echo "   ./comprehensive_benchmark"
echo ""
echo "📖 For detailed findings, see:"
echo "   doc/ZFRAMEWORK_PERFORMANCE_REPORT.md"