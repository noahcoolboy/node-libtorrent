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
              ['OS=="win"', {'action': ['echo "%cd%"']}],
              ['OS!="win"', {'action': ['node', 'compile.js']}]
          ]
        }
      ]
    }
  ]
}
