{
  'targets': [
    {
      'target_name': 'cmakebuild',
      'target_type': 'none',
      'actions': [
        {
          'action_name': 'cmakejsaction',
          'inputs': ['CMakeLists.txt'],
          'outputs': ['build'],
          'conditions': [
              ['OS=="win"', {'action': ['invokecmake.bat']}],
              ['OS!="win"', {'action': ['node', 'compile.js']}]
          ]
        }
      ]
    }
  ]
}
